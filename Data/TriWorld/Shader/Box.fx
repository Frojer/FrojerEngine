#include "../../Standard/FEHLSLDefine.fx"

//VS ��� ����ü.
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

    o.col = FELighting(pos, nor);

	// ��ȯ
    //pos = mul(pos, mWorld);
    //pos = mul(pos, mView);
    //pos = mul(pos, mProj);	 	

    //pos = mul(pos, mWVP);

    float4 N, L;
    
    pos = mul(pos, mWV);
    //N = mul(nor, mWV);
    //L = light[0].direction;
    //L = mul(vLightLocalDir[0], mWorld);

    N = nor;
    L = vLightLocalDir[0];
    
    o.col = dot(N, L);

    //diff += light[i].diffuse * diffuse * abs(dot(nor, vLightLocalDir[i]));

    pos = mul(pos, mProj);

	o.pos = pos;
	o.nor = nor;
    o.uv = uv;

    return o;
}

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main
//
float4 PS_Main(v2p i) : SV_TARGET
{
    float4 diff = texDiff.Sample(smp, i.uv);
    float4 moss = texMoss.Sample(smp, i.uv);
    float4 mask = texMask.Sample(smp, i.uv);
    float4 snow = texSnow.Sample(smp, i.uv);



    return diff;
}



/**************** end of file "Demo.fx" ***********************/
