#define FE_TEXTURE_SIZE 16
#define FE_LIGHT_SIZE 5

struct Texture
{
    float4 ot;
    float4 angle_Amount;
};
struct Light
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float4 position;
    float4 direction;
    float range;
    uint lightType;
    bool useLight;
};

cbuffer WVP_Data : register(b0)
{
    matrix mWorld;
    matrix mView;
    matrix mWV;
    matrix mProj;
}
cbuffer Texture_Data : register(b1)
{
    Texture texInfo[FE_TEXTURE_SIZE];
}
cbuffer Light_Data : register(b2)
{
    Light light[FE_LIGHT_SIZE];
}

float4 FELight(float4 pos, float4 nor)
{

}