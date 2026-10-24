struct Input {
    float4 position : SV_Position;
    float3 normal : TEXCOORD0;
    float3 color : TEXCOORD1;
};
float4 main(Input input) : SV_Target0 {
    float brightness = 0.22f + 0.78f * max(dot(normalize(input.normal),
        normalize(float3(-0.4f, -0.6f, 0.8f))), 0.0f);
    return float4(input.color * brightness, 1.0f);
}
