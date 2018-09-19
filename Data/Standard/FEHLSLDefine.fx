#define FE_LIGHT_SIZE 5

struct TextureInfo
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

cbuffer LightCB : register(b0)
{
    Light light[FE_LIGHT_SIZE];
}
cbuffer CameraCB : register(b1)
{
    matrix mView;
    matrix mProj;
};
cbuffer MaterialCB : register(b2)
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float power;
};
cbuffer ObjectCB : register(b3)
{
    matrix mPos;
    matrix mRot;
    matrix mScale;
    matrix mRP;
    matrix mWorld;
    matrix mWV;
    matrix mWVP;
    float4 vLightLocalPos[FE_LIGHT_SIZE];
    float4 vLightLocalDir[FE_LIGHT_SIZE];
};

float4 FELighting(float4 pos, float4 nor)
{
    float alpha = diffuse.a;
    float4 N = nor;
    float4 L;
    float4 diff = 0;
    float4 amb = 0;
    float dist;
    
    N.w = 0;

    for (uint i = 0; i < FE_LIGHT_SIZE; i++)
    {
        if (!light[i].useLight)
            continue;

        switch (light[i].lightType)
        {
            // directional light
            case 0:
                //L = vLightLocalDir[i];
                //diff += max(dot(N, L), 0) * light[i].diffuse * diffuse;
                //amb = ambient * light[i].ambient;
                break;

            // point light
            case 1:
                // 로컬 라이팅
                L = pos - vLightLocalPos[i];
                dist = length(vLightLocalPos[i] - pos);
                //L = pos - mul(light[i].position, mView);
                //dist = length(mul(light[i].position, mView) - pos);
                diff = 1 - min((dist / light[i].range), 1);
                break;

            // spot light
            case 2:
                break;
        }
    }

    diff.a = alpha;
    amb.a = 0;

    return diff + amb;
}