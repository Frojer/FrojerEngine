#include "FEHLSLDefine.fx"

cbuffer ConstBuffer
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float power;
};

//VS 출력 구조체.
struct v2p
{
    float4 pos : SV_POSITION;
    float4 nor : NORMAL;
};


////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main
//
v2p VS_Main(float4 pos : POSITION,
            float4 nor : NORMAL)
{
    v2p o = (v2p)0;
    pos.w = 1.0f;

	// 변환
    pos = mul(pos, mWorld);
    pos = mul(pos, mView);
    pos = mul(pos, mProj);	 	

	o.pos = pos;
	o.nor = nor;

    return o;
}





////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main
//
float4 PS_Main(v2p i) : SV_TARGET
{
	//float4 col2 = {1, 0, 1, 1};
	 
    return diffuse;
}



/**************** end of file "Demo.fx" ***********************/
