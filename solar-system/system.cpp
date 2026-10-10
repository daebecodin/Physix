
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include "physix.h"
#include "physix_calulations.h"
#include "physix_constants.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <iostream>

// Render to Draw
static SDL_Window* physixWindow = nullptr;
static SDL_Renderer* physixRenderer = nullptr;
static SDL_Surface* physixSurface = nullptr;
static SDL_Texture* physixTexture = nullptr;
static SDL_Event physixEvent;

constexpr float pixelsPerMeter { 100.0 / Physix::Distances::EARTH_ORBITAL_DISTANCE };

static Uint64 lastTime {};

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define CIRCLE_DRAW_SIDES 32
#define CIRCLE_DRAW_SIDES_LEN (CIRCLE_DRAW_SIDES + 1)

typedef struct {

} GameState;

static void drawCircle(SDL_Renderer *renderer, float r, float x, float y)
{
    float ang;
    SDL_FPoint points[CIRCLE_DRAW_SIDES_LEN];
    int i;
    for (i = 0; i < CIRCLE_DRAW_SIDES_LEN; i++) {
        ang = 2.0f * SDL_PI_F * (float)i / (float)CIRCLE_DRAW_SIDES;
        points[i].x = x + r * SDL_cosf(ang);
        points[i].y = y + r * SDL_sinf(ang);
    }
    SDL_RenderLines(renderer, (const SDL_FPoint*)&points, CIRCLE_DRAW_SIDES_LEN);
}


int main(int argc, char* argv[]) 
{
    bool isRunning = true;

    SDL_SetAppMetadata("Physix Simulations", "1.0", "daebecodin-physix");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Initialization Failed: %s", SDL_GetError());
        return 3;
    }

    if(!SDL_CreateWindowAndRenderer("Physix", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &physixWindow, &physixRenderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window Initialization Faild: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    SDL_SetRenderLogicalPresentation(physixRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    namespace Masses = Physix::Masses;
    namespace Distances = Physix::Distances;
    namespace Calculate = Physix::Orbital;
    namespace System = Physix;


    Body sun {
        "Sun",
            Masses::SUN_MASS, 
            {0.0, 0.0, 0.0}, 
            {}
    };

    Body mercury {
        "Mercury",
            Masses::MERCURY_MASS, 
            {Distances::MERCURY_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    Body venus {
        "Venus",
            Masses::VENUS_MASS,
            {Distances::VENUS_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    Body earth {
        "Earth",
            Masses::EARTH_MASS,
            {Distances::EARTH_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    Body mars {
        "Mars",
            Masses::MARS_MASS,
            {Distances::MARS_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    Body jupiter {
        "Jupiter",
            Masses::JUPITER_MASS,
            {Distances::JUPITER_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    Body saturn {
        "Saturn",
            Masses::SATURN_MASS,
            {Distances::SATURN_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    Body uranus {
        "Uranus", 
            Masses::URANUS_MASS,
            {Distances::URANUS_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    Body neptune {
        "Neptune",
            Masses::NEPTUNE_MASS,
            {Distances::NEPTUNE_ORBITAL_DISTANCE, 0.0, 0.0},
            {}
    };

    // Circular Orbit Speeds and Initial Velocity
    double mercuryOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::MERCURY_ORBITAL_DISTANCE);
    mercury.velocity = {0.0, mercuryOrbitSpeed, 0.0};

    double venusOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::VENUS_ORBITAL_DISTANCE);
    venus.velocity = {0.0, venusOrbitSpeed, 0.0};

    double earthOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::EARTH_ORBITAL_DISTANCE);
    earth.velocity = {0.0, earthOrbitSpeed, 0.0};

    double marsOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::MARS_ORBITAL_DISTANCE);
    mars.velocity = {0.0, marsOrbitSpeed, 0.0};

    double jupiterOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::JUPITER_ORBITAL_DISTANCE);
    jupiter.velocity = {0.0, jupiterOrbitSpeed, 0.0};

    double saturnOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::SATURN_ORBITAL_DISTANCE);
    saturn.velocity = {0.0, saturnOrbitSpeed, 0.0};

    double uranusOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::URANUS_ORBITAL_DISTANCE);
    uranus.velocity = {0.0, uranusOrbitSpeed, 0.0};

    double neptuneOrbitSpeed = Calculate::circularOrbitSpeed(sun.mass, Distances::NEPTUNE_ORBITAL_DISTANCE);
    neptune.velocity = {0.0, neptuneOrbitSpeed, 0.0};



    // make a world struct
    std::vector<Body> bodies = {sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune};
    const std::vector<Body> startingBodies = bodies;

    double duration = 365.0 * 24.0 * 60.0 * 60.0; // 86,400 seconds
    double dt = 60.0;// 60 seconds per step
    double elapsedTime = 0.0;

    std::cout << "\nStarting Positions (m)\n"
        << std::left << std::setw(8) << "Body"
        << std::right << std::setw(14) << "X Pos" 
        << std::setw(14) << "Y pos" 
        << std::setw(14) << "Z pos" << '\n';

    for (const Body& body : startingBodies)
    {
        std::cout << std::left << std::setw(8) << body.name
            << std::right << std::scientific << std::setprecision(4)
            << std::setw(14) << body.position.x
            << std::setw(14) << body.position.y
            << std::setw(14) << body.position.z << '\n';
    }


    std::cout << '\n'
        << std::left << std::setw(8) << "Body"
        << std::right << std::setw(14) << "Sun dist (km)"
        << std::setw(14) << " X pos (km)"
        << std::setw(14) << "Y pos (km)" << '\n';

    while (isRunning) 
    {
        // queue events
        while (SDL_PollEvent(&physixEvent))
        {
            if (physixEvent.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
        }

        if (!isRunning)
        {
            break;
        }

        if (elapsedTime < duration)
        {
            for (int step = 0; step < 100 && elapsedTime < duration; ++step)
            {
                System::simulateSystem(bodies, dt);
                elapsedTime += dt;
            }

            if (elapsedTime > dt) 
            {
                std::cout << "\033[" << bodies.size() + 1 << "A";
            }

            std::cout << '\r' << "\033[2K"
                << "Days: " 
                << std::fixed << std::setprecision(3)
                << elapsedTime / 86400.0 << '\n';

            for (std::size_t j = 0; j < bodies.size(); ++j)
            {
                std::cout << '\r' << "\033[2K"
                    << std::left << std::setw(8) << bodies[j].name
                    << std::right << std::scientific
                    << std::setprecision(4)
                    << std::setw(14)
                    << Calculate::distanceFromSun(bodies, j) / 1000.0
                    << std::setw(14)
                    << bodies[j].position.x / 1000.0
                    << std::setw(14)
                    << bodies[j].position.y / 1000.0
                    << '\n';
            }

            std::cout << std::flush;
        }

        // Drawing current State
        SDL_SetRenderDrawColor(physixRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(physixRenderer);

        // Draw Bodies - 
        SDL_SetRenderDrawColor(physixRenderer, 80, 140, 255, 255);

        // window origin
        const float centerX = WINDOW_WIDTH / 2.0f;
        const float centerY = WINDOW_HEIGHT / 2.0f;

        for (Body body : bodies) 
        {
            // where to render current body
            const float screenX = static_cast<float> ( centerX + body.position.x * pixelsPerMeter);
            const float screenY = static_cast<float> ( centerY - body.position.y * pixelsPerMeter);


            drawCircle(physixRenderer, 5.0f, screenX, screenY);

        }

        // Draw Particles - SDL_RenderPoints

        // Update screen
        SDL_RenderPresent(physixRenderer);

        // Delay data
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }

    SDL_DestroyRenderer(physixRenderer);
    SDL_DestroyWindow(physixWindow);

    SDL_Quit();
    return 0;

} 
