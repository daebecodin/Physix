struct Body { float4 position_mass; float4 velocity; };
StructuredBuffer<Body> bodies : register(t0, space0);
cbuffer View : register(b0, space1) {
    float aspect;
    float unused0;
    float unused1;
    float unused2;
};
struct Output {
    float4 position : SV_Position;
    float3 normal : TEXCOORD0;
    float3 color : TEXCOORD1;
};
float3 point(uint row, uint column) {
    float latitude = row * 3.14159265359f / 20.0f;
    float longitude = column * 6.28318530718f / 40.0f;
    return float3(sin(latitude) * cos(longitude),
                  sin(latitude) * sin(longitude), cos(latitude));
}
Output main(uint id : SV_VertexID, uint instance : SV_InstanceID) {
    uint cell = id / 6;
    uint corner = id % 6;
    uint row = cell / 40;
    uint column = cell % 40;
    uint dr = (corner == 1 || corner == 2 || corner == 4) ? 1 : 0;
    uint dc = (corner == 2 || corner == 4 || corner == 5) ? 1 : 0;
    float3 normal = point(row + dr, column + dc);
    float radius = instance == 0 ? 0.105f : 0.045f;
    float3 world = bodies[instance].position_mass.xyz + radius * normal;
    float3 relative = world - float3(0.5f, 0.0f, 0.0f);
    float depth = 3.2f + relative.y * 0.868f - relative.z * 0.496f;
    Output out;
    out.position = float4(relative.x / (0.57735f * aspect),
                          (relative.y * 0.496f + relative.z * 0.868f) / 0.57735f,
                          depth * depth / 10.0f, depth);
    out.normal = normal;
    out.color = instance == 0 ? float3(1.0f, 0.56f, 0.12f) : float3(0.20f, 0.54f, 1.0f);
    return out;
}
