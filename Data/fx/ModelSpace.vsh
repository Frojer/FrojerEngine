
// ��� ���� Constant Buffer Variables
cbuffer ConstBuffer //: register(b0) //������� 0���� ��� (�ִ� 14��. 0~13)
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
    matrix mWVP;
}




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
    pos.w = 1.0f;

	 //��ȯ.
    pos = mul(pos, mWVP);
    //pos = mul(pos, mWorld);
    //pos = mul(pos, mView);
    //pos = mul(pos, mProj);	 	

    o.pos = pos; //��ȯ�� ���� ���..
    o.col = col; //������ ��ȯ ���� ���.

    return o;
}