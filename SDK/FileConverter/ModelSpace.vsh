
// 상수 버퍼 Constant Buffer Variables
cbuffer DefaultConstBuffer : register(b0) //상수버퍼 0번에 등록 (최대 14개. 0~13)
{
    matrix mWorld;
    matrix mView;
    matrix mWV;
    matrix mProj;
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
				  float4 pos : POSITION, //[입력] 정점좌표. Vertex Position (Model Space, 3D)
				  float4 col : COLOR0 //[입력] 정점색 Vertex Color	: "Diffuse"	
				)
{

    VSOutput o = (VSOutput) 0;
    pos.w = 1.0f;

	 //변환.
    //pos = mul(pos, mWVP);
    pos = mul(pos, mWorld);
    pos = mul(pos, mView);
    pos = mul(pos, mProj);	 	

    o.pos = pos; //변환된 정보 출력..
    o.col = col;
    //o.col = abs(mWorld[3]); //색상은 변환 없이 출력.

    return o;
}