struct Body { float4 position_mass; float4 velocity; float4 appearance; };
StructuredBuffer<Body> input : register(t0, space0);
RWStructuredBuffer<Body> output : register(u0, space1);
cbuffer Parameters : register(b0, space2) {
    float gravity;
    float dt;
    uint body_count;
    uint phase;
};

float3 acceleration(uint i) {
    float3 result = float3(0.0f, 0.0f, 0.0f);
    float3 position = input[i].position_mass.xyz;
    for (uint j = 0; j < body_count; ++j) {
        if (j == i) continue;
        float3 delta = input[j].position_mass.xyz - position;
        float r2 = max(dot(delta, delta), 1.0e-8f);
        result += gravity * input[j].position_mass.w * delta * rsqrt(r2 * r2 * r2);
    }
    return result;
}

[numthreads(64, 1, 1)]
void main(uint3 id : SV_DispatchThreadID) {
    uint i = id.x;
    if (i >= body_count) return;
    Body body = input[i];
    float3 a = acceleration(i);
    if (phase == 0) {
        body.velocity.xyz += a * (0.5f * dt);
        body.position_mass.xyz += body.velocity.xyz * dt;
    } else {
        body.velocity.xyz += a * (0.5f * dt);
    }
    output[i] = body;
}
