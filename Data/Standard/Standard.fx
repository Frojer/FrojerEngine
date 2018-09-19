#include "FEHLSLDefine.fx"

//VS 출력 구조체.
struct v2p
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 pos3d : TEXCOORD1;
    float4 nor3d : TEXCOORD2;
};

Texture2D tex : register(t0);
SamplerState smp;

v2p VS_Main(float4 pos : POSITION,
            float4 nor : NORMAL,
            float2 uv : TEXCOORD0)
{
    v2p o = (v2p)0;

    pos.w = 1.0f;
    nor.w = 0.0f;
    
    o.pos3d = mul(pos, mWV);
    o.nor3d = normalize(mul(nor, mWV));
    o.pos = mul(o.pos3d, mProj);
    o.uv = uv;

    return o;
}

float4 PS_Main(v2p i) : SV_TARGET
{
    float4 diff;
    float4 texDiff = tex.Sample(smp, i.uv);
	
    diff = FELighting(i.pos3d, i.nor3d);

    clip(texDiff.a < 0.25 ? -1 : 1);
    
    return texDiff * diff;
}