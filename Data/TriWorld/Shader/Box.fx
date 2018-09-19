#include "../../Standard/FEHLSLDefine.fx"

//VS 출력 구조체.
struct v2p
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float4 nor : NORMAL;
    float2 uv : TEXCOORD0;
};

cbuffer ConstantBuffer
{
    
}

Texture2D texDiff : register(t0);
Texture2D texMoss : register(t1);
Texture2D texMask : register(t2);
Texture2D texSnow : register(t3);
SamplerState smp;

v2p VS_Main(float4 pos : POSITION,
            float4 nor : NORMAL,
            float2 uv : TEXCOORD0)
{
    v2p o = (v2p)0;
    pos.w = 1.0f;

    o.col = FELighting(pos, nor);
    
    pos = mul(pos, mWVP);

	o.pos = pos;
	o.nor = nor;
    o.uv = uv;

    return o;
}

float4 PS_Main(v2p i) : SV_TARGET
{
    float4 diff = texDiff.Sample(smp, i.uv);
    float4 moss = texMoss.Sample(smp, i.uv);
    float4 mask = texMask.Sample(smp, i.uv);
    float4 snow = texSnow.Sample(smp, i.uv);

    return diff * i.col;
}