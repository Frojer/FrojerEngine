#include "../../Standard/FEHLSLDefine.fx"

Texture2D tex : register(t0);
SamplerState smp;

float4 VS_Main(float4 pos : POSITION,
            float4 nor : NORMAL,
            float2 uv : TEXCOORD0) : SV_Position
{
    pos.w = 1.0f;

    pos = mul(pos, mWVP);

    return pos;
}

float4 PS_Main() : SV_TARGET
{
    return diffuse;
}