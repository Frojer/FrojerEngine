#include "FEHLSLDefine.fx"

cbuffer ConstBuffer
{

};

//VS 출력 구조체.
struct v2p
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
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
    nor.w = 0.0f;

    o.col = FELighting(pos, nor);

	// 변환
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

    return o;
}





////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main
//
float4 PS_Main(v2p i) : SV_TARGET
{
	//float4 col2 = {1, 0, 1, 1};
	
    //return 1;
    //return vLightLocalDir[0];
    //return light[0].ambient;
    //return light[0].diffuse;
    return i.col;
}



/**************** end of file "Demo.fx" ***********************/
