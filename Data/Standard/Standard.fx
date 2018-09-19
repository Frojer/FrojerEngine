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


////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main
//
v2p VS_Main(float4 pos : POSITION,
            float4 nor : NORMAL,
            float2 uv : TEXCOORD0)
{
    v2p o = (v2p)0;

    pos.w = 1.0f;
    nor.w = 0.0f;
    
    //pos = mul(pos, mWV);
    //nor = mul(nor, mWV);
    
    o.pos3d = pos;
    o.nor3d = nor;

    //pos = mul(pos, mProj);

    /*// 로컬 라이팅
    N = nor;
    L = vLightLocalDir[0];
    o.col = dot(N, L);
    //*/

    /*// 뷰공간 라이팅
    nor.w = 0;
    N = mul(nor, mWV);
    N = normalize(N);
    L = light[0].direction;
    L.w = 0;
    L = mul(L, mView);
    o.col = dot(N, L);
    //*/

	o.pos = pos;
    o.uv = uv;

    return o;
}

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main
//
float4 PS_Main(v2p i) : SV_TARGET
{
    float4 diff;
    float4 texDiff = tex.Sample(smp, i.uv);
	
    diff = FELighting(i.pos3d, i.nor3d);

    clip(texDiff.a < 0.25 ? -1 : 1);
    
    return diff;
}