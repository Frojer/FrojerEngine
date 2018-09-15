cbuffer VP : register(b0)
{
    matrix mVP;
    float4 color;
};

float4 VS_Main(float4 pos : POSITION) : SV_POSITION
{
    pos.w = 1;
    pos = mul(pos, mVP);

    return pos;
}

float4 PS_Main() : SV_TARGET
{
    return color;
}