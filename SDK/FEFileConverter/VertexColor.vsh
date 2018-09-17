
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
    o.pos = pos; //입력된 정보 출력..
    o.col = col;

    return o;
}
