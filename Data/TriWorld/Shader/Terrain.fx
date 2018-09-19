#include "../../Standard/FEHLSLDefine.fx"

//VS 출력 구조체.
struct v2p
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 pos3d : TEXCOORD1;
    float4 nor3d : TEXCOORD2;
};

cbuffer ConstantBuffer
{
    float4 ot;
};

Texture2D tex : register(t0);
SamplerState smp;

v2p VS_Main(float4 pos : POSITION,
            float4 nor : NORMAL,
            float2 uv : TEXCOORD0)
{
    v2p o = (v2p) 0;
    pos.w = 1.0f;
    nor.w = 0.0f;

    o.pos3d = pos;
    o.nor3d = nor;
    
    pos = mul(pos, mWVP);

    o.pos = pos;
    o.uv = (uv + ot.xy) * ot.zw;

    return o;
}

float4 PS_Main(v2p i) : SV_TARGET
{
    float4 diff;
    float4 texDiff = tex.Sample(smp, i.uv);

    diff = FELighting(i.pos3d, i.nor3d);
    
    return diff;
}