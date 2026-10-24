#include <SDL3/SDL.h>
#include "physix_calulations.h"
#include "physix_constants.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

namespace {
constexpr float au = static_cast<float>(Physix::Distances::EARTH_ORBITAL_DISTANCE);
constexpr float day = 86400.0f;
constexpr int width = 1280;
constexpr int height = 720;
constexpr int latitude_cells = 20;
constexpr int longitude_cells = 40;
constexpr int sphere_vertices = latitude_cells * longitude_cells * 6;
constexpr float step_days = 0.5f;
constexpr int steps_per_frame = 2;

struct alignas(16) Four { float x, y, z, w; };
struct alignas(16) BodyState { Four position_mass; Four velocity; };
struct alignas(16) PhysicsParameters { float gravity, dt, unused0, unused1; };
struct alignas(16) ViewParameters { float aspect, unused0, unused1, unused2; };
static_assert(sizeof(BodyState) == 32);

struct App {
    SDL_Window* window = nullptr;
    SDL_GPUDevice* device = nullptr;
    SDL_GPUBuffer* states[2] {};
    SDL_GPUComputePipeline* compute = nullptr;
    SDL_GPUGraphicsPipeline* graphics = nullptr;
    SDL_GPUTexture* depth = nullptr;
    int depth_width = 0, depth_height = 0;
    int current = 0;

    ~App() {
        if (device) {
            SDL_WaitForGPUIdle(device);
            if (depth) SDL_ReleaseGPUTexture(device, depth);
            if (graphics) SDL_ReleaseGPUGraphicsPipeline(device, graphics);
            if (compute) SDL_ReleaseGPUComputePipeline(device, compute);
            for (auto* buffer : states) if (buffer) SDL_ReleaseGPUBuffer(device, buffer);
            if (window) SDL_ReleaseWindowFromGPUDevice(device, window);
        }
        if (window) SDL_DestroyWindow(window);
        if (device) SDL_DestroyGPUDevice(device);
        SDL_Quit();
    }
};

void require(bool okay, const char* operation) {
    if (!okay) throw std::runtime_error(std::string(operation) + ": " + SDL_GetError());
}

struct ShaderFile {
    void* bytes = nullptr;
    size_t size = 0;
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
    const char* entry = "main";
    ShaderFile() = default;
    ShaderFile(const ShaderFile&) = delete;
    ShaderFile& operator=(const ShaderFile&) = delete;
    ShaderFile(ShaderFile&& other) noexcept
        : bytes(other.bytes), size(other.size), format(other.format), entry(other.entry) {
        other.bytes = nullptr;
    }
    ~ShaderFile() { SDL_free(bytes); }
};

ShaderFile load_shader(SDL_GPUDevice* device, const char* stem) {
    ShaderFile result;
    const auto formats = SDL_GetGPUShaderFormats(device);
    const char* folder = nullptr;
    const char* extension = nullptr;
    if (formats & SDL_GPU_SHADERFORMAT_MSL) {
        result.format = SDL_GPU_SHADERFORMAT_MSL;
        folder = "MSL"; extension = "msl"; result.entry = "main0";
    } else if (formats & SDL_GPU_SHADERFORMAT_SPIRV) {
        result.format = SDL_GPU_SHADERFORMAT_SPIRV;
        folder = "SPIRV"; extension = "spv";
    } else if (formats & SDL_GPU_SHADERFORMAT_DXIL) {
        result.format = SDL_GPU_SHADERFORMAT_DXIL;
        folder = "DXIL"; extension = "dxil";
    } else {
        throw std::runtime_error("No supported SDL GPU shader format");
    }
    const std::string path = std::string(SDL_GetBasePath()) + "shaders/" + folder + "/" + stem + "." + extension;
    result.bytes = SDL_LoadFile(path.c_str(), &result.size);
    require(result.bytes != nullptr, path.c_str());
    return result;
}

void make_pipelines(App& app) {
    auto compute_file = load_shader(app.device, "step.comp");
    SDL_GPUComputePipelineCreateInfo ci {};
    ci.code = static_cast<const Uint8*>(compute_file.bytes);
    ci.code_size = compute_file.size;
    ci.entrypoint = compute_file.entry;
    ci.format = compute_file.format;
    ci.num_readonly_storage_buffers = 1;
    ci.num_readwrite_storage_buffers = 1;
    ci.num_uniform_buffers = 1;
    ci.threadcount_x = 2; ci.threadcount_y = 1; ci.threadcount_z = 1;
    app.compute = SDL_CreateGPUComputePipeline(app.device, &ci);
    require(app.compute != nullptr, "Create compute pipeline");

    auto vertex_file = load_shader(app.device, "sphere.vert");
    auto fragment_file = load_shader(app.device, "sphere.frag");
    SDL_GPUShaderCreateInfo vi {};
    vi.code = static_cast<const Uint8*>(vertex_file.bytes);
    vi.code_size = vertex_file.size; vi.entrypoint = vertex_file.entry;
    vi.format = vertex_file.format; vi.stage = SDL_GPU_SHADERSTAGE_VERTEX;
    vi.num_storage_buffers = 1; vi.num_uniform_buffers = 1;
    SDL_GPUShader* vertex = SDL_CreateGPUShader(app.device, &vi);
    require(vertex != nullptr, "Create vertex shader");
    SDL_GPUShaderCreateInfo fi {};
    fi.code = static_cast<const Uint8*>(fragment_file.bytes);
    fi.code_size = fragment_file.size; fi.entrypoint = fragment_file.entry;
    fi.format = fragment_file.format; fi.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    SDL_GPUShader* fragment = SDL_CreateGPUShader(app.device, &fi);
    if (!fragment) { SDL_ReleaseGPUShader(app.device, vertex); require(false, "Create fragment shader"); }

    SDL_GPUColorTargetDescription color {};
    color.format = SDL_GetGPUSwapchainTextureFormat(app.device, app.window);
    SDL_GPUGraphicsPipelineCreateInfo pi {};
    pi.vertex_shader = vertex; pi.fragment_shader = fragment;
    pi.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    pi.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    pi.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
    pi.multisample_state.sample_count = SDL_GPU_SAMPLECOUNT_1;
    pi.depth_stencil_state.enable_depth_test = true;
    pi.depth_stencil_state.enable_depth_write = true;
    pi.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
    pi.target_info.color_target_descriptions = &color;
    pi.target_info.num_color_targets = 1;
    pi.target_info.has_depth_stencil_target = true;
    pi.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM;
    app.graphics = SDL_CreateGPUGraphicsPipeline(app.device, &pi);
    SDL_ReleaseGPUShader(app.device, vertex);
    SDL_ReleaseGPUShader(app.device, fragment);
    require(app.graphics != nullptr, "Create graphics pipeline");
}

void make_depth(App& app, int w, int h) {
    if (app.depth && app.depth_width == w && app.depth_height == h) return;
    if (app.depth) SDL_ReleaseGPUTexture(app.device, app.depth);
    SDL_GPUTextureCreateInfo info {};
    info.type = SDL_GPU_TEXTURETYPE_2D;
    info.format = SDL_GPU_TEXTUREFORMAT_D16_UNORM;
    info.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    info.width = static_cast<Uint32>(w); info.height = static_cast<Uint32>(h);
    info.layer_count_or_depth = 1; info.num_levels = 1;
    info.sample_count = SDL_GPU_SAMPLECOUNT_1;
    app.depth = SDL_CreateGPUTexture(app.device, &info);
    require(app.depth != nullptr, "Create depth texture");
    app.depth_width = w; app.depth_height = h;
}

void initialize_states(App& app) {
    constexpr Uint32 bytes = sizeof(BodyState) * 2;
    SDL_GPUBufferCreateInfo bi {};
    bi.size = bytes;
    bi.usage = SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ |
               SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE |
               SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ;
    for (auto& buffer : app.states) {
        if (!buffer) buffer = SDL_CreateGPUBuffer(app.device, &bi);
        require(buffer != nullptr, "Create state buffer");
    }

    const double orbital_speed = Physix::Orbital::circularOrbitSpeed(
        Physix::Masses::SUN_MASS + Physix::Masses::EARTH_MASS,
        Physix::Distances::EARTH_ORBITAL_DISTANCE);
    const float earth_speed = static_cast<float>(orbital_speed * day / au);
    const float ratio = static_cast<float>(Physix::Masses::EARTH_MASS / Physix::Masses::SUN_MASS);
    const std::array<BodyState, 2> initial {{
        {{0, 0, 0, 1}, {0, -earth_speed * ratio, 0, 0}},
        {{1, 0, 0, ratio}, {0, earth_speed, 0, 0}}
    }};
    SDL_GPUTransferBufferCreateInfo ti {};
    ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD; ti.size = bytes;
    auto* transfer = SDL_CreateGPUTransferBuffer(app.device, &ti);
    require(transfer != nullptr, "Create upload buffer");
    void* mapped = SDL_MapGPUTransferBuffer(app.device, transfer, false);
    require(mapped != nullptr, "Map upload buffer");
    std::memcpy(mapped, initial.data(), bytes);
    SDL_UnmapGPUTransferBuffer(app.device, transfer);
    auto* cmd = SDL_AcquireGPUCommandBuffer(app.device);
    require(cmd != nullptr, "Acquire upload command buffer");
    auto* copy = SDL_BeginGPUCopyPass(cmd);
    require(copy != nullptr, "Begin upload pass");
    SDL_GPUTransferBufferLocation src {transfer, 0};
    SDL_GPUBufferRegion dst {app.states[0], 0, bytes};
    SDL_UploadToGPUBuffer(copy, &src, &dst, false);
    SDL_EndGPUCopyPass(copy);
    require(SDL_SubmitGPUCommandBuffer(cmd), "Submit upload");
    SDL_ReleaseGPUTransferBuffer(app.device, transfer);
    app.current = 0;
}

void frame(App& app, bool paused) {
    auto* cmd = SDL_AcquireGPUCommandBuffer(app.device);
    require(cmd != nullptr, "Acquire frame command buffer");
    if (!paused) {
        constexpr PhysicsParameters parameters {
            static_cast<float>(Physix::GravitationalForces::UNIVERSAL_GRAVITY *
                Physix::Masses::SUN_MASS * 86400.0 * 86400.0 /
                (Physix::Distances::EARTH_ORBITAL_DISTANCE *
                 Physix::Distances::EARTH_ORBITAL_DISTANCE *
                 Physix::Distances::EARTH_ORBITAL_DISTANCE)),
            step_days, 0, 0
        };
        for (int step = 0; step < steps_per_frame; ++step) {
            const int next = 1 - app.current;
            SDL_GPUStorageBufferReadWriteBinding output {app.states[next], false, 0, 0, 0};
            auto* pass = SDL_BeginGPUComputePass(cmd, nullptr, 0, &output, 1);
            require(pass != nullptr, "Begin compute pass");
            SDL_BindGPUComputePipeline(pass, app.compute);
            SDL_BindGPUComputeStorageBuffers(pass, 0, &app.states[app.current], 1);
            SDL_PushGPUComputeUniformData(cmd, 0, &parameters, sizeof(parameters));
            SDL_DispatchGPUCompute(pass, 1, 1, 1);
            SDL_EndGPUComputePass(pass);
            app.current = next;
        }
    }

    SDL_GPUTexture* swapchain = nullptr;
    Uint32 w = 0, h = 0;
    require(SDL_WaitAndAcquireGPUSwapchainTexture(cmd, app.window, &swapchain, &w, &h), "Acquire swapchain");
    if (swapchain && w && h) {
        make_depth(app, static_cast<int>(w), static_cast<int>(h));
        SDL_GPUColorTargetInfo color {};
        color.texture = swapchain;
        color.clear_color = {0.012f, 0.019f, 0.043f, 1.0f};
        color.load_op = SDL_GPU_LOADOP_CLEAR;
        color.store_op = SDL_GPU_STOREOP_STORE;
        SDL_GPUDepthStencilTargetInfo depth {};
        depth.texture = app.depth;
        depth.clear_depth = 1.0f;
        depth.load_op = SDL_GPU_LOADOP_CLEAR;
        depth.store_op = SDL_GPU_STOREOP_DONT_CARE;
        auto* pass = SDL_BeginGPURenderPass(cmd, &color, 1, &depth);
        require(pass != nullptr, "Begin render pass");
        SDL_BindGPUGraphicsPipeline(pass, app.graphics);
        SDL_BindGPUVertexStorageBuffers(pass, 0, &app.states[app.current], 1);
        ViewParameters view {static_cast<float>(w) / static_cast<float>(h), 0, 0, 0};
        SDL_PushGPUVertexUniformData(cmd, 0, &view, sizeof(view));
        SDL_DrawGPUPrimitives(pass, sphere_vertices, 2, 0, 0);
        SDL_EndGPURenderPass(pass);
    }
    require(SDL_SubmitGPUCommandBuffer(cmd), "Submit frame");
}
}

int main(int, char**) {
    try {
        App app;
        require(SDL_Init(SDL_INIT_VIDEO), "SDL_Init");
        app.device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL |
            SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, true, nullptr);
        require(app.device != nullptr, "Create GPU device");
        app.window = SDL_CreateWindow("Physix | GPU two-body (Space pause, R reset, Esc quit)",
            width, height, SDL_WINDOW_RESIZABLE);
        require(app.window != nullptr, "Create window");
        require(SDL_ClaimWindowForGPUDevice(app.device, app.window), "Claim GPU window");
        make_pipelines(app);
        initialize_states(app);
        bool running = true, paused = false;
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) running = false;
                if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                    if (event.key.key == SDLK_ESCAPE) running = false;
                    if (event.key.key == SDLK_SPACE) paused = !paused;
                    if (event.key.key == SDLK_R) { SDL_WaitForGPUIdle(app.device); initialize_states(app); }
                }
            }
            if (running) frame(app, paused);
            SDL_Delay(16);
        }
        return 0;
    } catch (const std::exception& error) {
        SDL_Log("Two-body GPU simulation: %s", error.what());
        return 1;
    }
}
