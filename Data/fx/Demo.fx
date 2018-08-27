//
//
// Demo.fx : 기본 셰이더 소스.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//


// 상수 버퍼 Constant Buffer Variables
cbuffer ConstBuffer //: register(b0) //상수버퍼 0번에 등록 (최대 14개. 0~13)
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
    matrix mWVP;
}




//VS 출력 구조체.
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
};



////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main
//
VSOutput VS_Main(
				  float4 pos : POSITION,    //[입력] 정점좌표. Vertex Position (Model Space, 3D)
				  float4 col : COLOR0       //[입력] 정점색 Vertex Color	: "Diffuse"	
				)
{

	VSOutput o = (VSOutput)0;
    pos.w = 1.0f;

	 //변환.
    pos = mul(pos, mWVP);	 
    //pos = mul(pos, mWorld);
    //pos = mul(pos, mView);
    //pos = mul(pos, mProj);	 	

	o.pos = pos;						//변환된 정보 출력..
	o.col = col;						//색상은 변환 없이 출력.

    return o;
}





////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main
//
float4 PS_Main(
				float4 pos : SV_POSITION,   //[입력] 정점좌표. (Screen, 2D)
				float4 col : COLOR0         //[입력] 정점색. (Vertex Color : "Diffuse")
				) : SV_TARGET
{
	//float4 col2 = {1, 0, 1, 1};
	 
    return col;
}



/**************** end of file "Demo.fx" ***********************/
