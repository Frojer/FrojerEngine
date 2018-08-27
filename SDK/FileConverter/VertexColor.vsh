
//VS ��� ����ü.
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
				  float4 pos : POSITION, //[�Է�] ������ǥ. Vertex Position (Model Space, 3D)
				  float4 col : COLOR0 //[�Է�] ������ Vertex Color	: "Diffuse"	
				)
{
    VSOutput o = (VSOutput) 0;
    o.pos = pos; //�Էµ� ���� ���..
    o.col = col;

    return o;
}
