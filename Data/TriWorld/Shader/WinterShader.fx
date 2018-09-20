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
    vector ot;
    vector AutumnColor;
    vector FogColor;
    uint seasonIndex;
    float FogNear;
    float FogFar;
}

Texture2D texDiff : register(t0);
Texture2D texWinter : register(t1);
SamplerState smp;

v2p VS_Main(float4 pos : POSITION,
            float4 nor : NORMAL,
            float2 uv : TEXCOORD0)
{
    v2p o = (v2p) 0;

    pos.w = 1.0f;
    nor.w = 0.0f;
    
    o.pos3d = mul(pos, mWV);
    o.nor3d = normalize(mul(nor, mWV));
    o.pos = mul(o.pos3d, mProj);
    o.uv = (uv + ot.xy) * ot.zw;

    return o;
}

float4 PS_Main(v2p i) : SV_TARGET
{
    float4 tex = texDiff.Sample(smp, i.uv);
    float4 winterMask = texWinter.Sample(smp, i.uv);
    float4 diff = 1;
    float fog = 0;

    switch (seasonIndex)
    {
        case 0:
            diff = tex;
            break;

        case 1:
            diff = tex * AutumnColor;
            break;

        case 2:
            diff = ((tex * (1 - winterMask)) + winterMask);
            break;
    }
	
    diff *= FELighting(i.pos3d, i.nor3d);
    
    diff.a = tex.a;

    clip(diff.a < 0.35f ? -1 : 1);
    
    fog = FEFogLinear(i.pos3d, FogNear, FogFar);

    return (diff * (1 - fog)) + (FogColor * fog);
}