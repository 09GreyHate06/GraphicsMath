cbuffer SystemCBuf : register(b0)
{
    float4x4 viewProjection;
};

cbuffer EntityBuf : register(b1)
{
    float4x4 transform;
};

struct VSOut
{
    float3 color : COLOR;
    float4 position : SV_POSITION;
};

VSOut main(float3 position : POSITION, float4 color : COLOR)
{
    VSOut vso;
    vso.position = mul(float4(position, 1.0f), mul(transform, viewProjection));
    vso.color = color;
    return vso;
}