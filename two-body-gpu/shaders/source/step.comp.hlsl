struct Body { float4 position_mass; float4 velocity; };
StructuredBuffer<Body> input : register(t0, space0);
RWStructuredBuffer<Body> output : register(u0, space1);
cbuffer Parameters : register(b0, space2) {
    float gravity;
    float dt;
    float unused0;
    float unused1;
};

float3 acceleration(float3 position, float3 other, float mass) {
    float3 delta = other - position;
    float r2 = max(dot(delta, delta), 1.0e-8f);
    return gravity * mass * delta * rsqrt(r2 * r2 * r2);
}

[numthreads(2, 1, 1)]
void main(uint3 id : SV_DispatchThreadID) {
    uint i = id.x;
    if (i >= 2) return;
    uint j = 1 - i;
    Body a = input[i], b = input[j];
    float3 a0 = acceleration(a.position_mass.xyz, b.position_mass.xyz, b.position_mass.w);
    float3 b0 = acceleration(b.position_mass.xyz, a.position_mass.xyz, a.position_mass.w);
    float3 half_a = a.velocity.xyz + a0 * (0.5f * dt);
    float3 half_b = b.velocity.xyz + b0 * (0.5f * dt);
    float3 new_a = a.position_mass.xyz + half_a * dt;
    float3 new_b = b.position_mass.xyz + half_b * dt;
    float3 a1 = acceleration(new_a, new_b, b.position_mass.w);
    a.position_mass.xyz = new_a;
    a.velocity.xyz = half_a + a1 * (0.5f * dt);
    output[i] = a;
}
