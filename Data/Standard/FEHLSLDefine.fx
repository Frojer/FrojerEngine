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
    matrix mWVNormal;
    matrix mWVP;
    float4 vLightLocalPos[FE_LIGHT_SIZE];
    float4 vLightLocalDir[FE_LIGHT_SIZE];
};

float4 FEMask(float4 a, float4 b, float mask)
{
    return (a * mask) + (b * (1 - mask));
}

float4 FELighting(float4 pos, float4 nor)
{
    float alpha = diffuse.a;
    float4 N = nor;
    float4 L;
    float4 lightPos;
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
                L = mul(light[i].direction, mView);
                diff += max(dot(N, L), 0) * diffuse * light[i].diffuse;
                amb = ambient * light[i].ambient;
                break;

            // point light
            case 1:
                lightPos = mul(light[i].position, mView);
                L = -normalize(pos - lightPos);
                dist = length(lightPos - pos);
                diff += max(dot(N, L), 0) * diffuse * light[i].diffuse * (1 - min((dist / light[i].range), 1));
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
float4 FESpecularLighting(float4 pos, float4 nor, float p)
{
    float4 N = nor;
    float4 L;
    float4 E;
    float4 H;
    float4 spec = 0;

    if (p < 0.0001)
        return 0;

    for (int i = 0; i < FE_LIGHT_SIZE; i++)
    {
        if (light[i].lightType != 0)
            continue;

        L = light[i].direction;
        L.w = 0;
        L = mul(L, mView);

		// 시선백터 계산.
        pos.w = 0;
        E = normalize(-pos);


		// 하프벡터 계산.
        H = normalize(L + E);

		// 조명 계산 
        spec += pow(max(dot(N, H), 0), p) * light[i].specular * specular;
    }

    spec.w = 1;

    return spec;
}
float FEFogLinear(float4 pos, float near, float far)
{
    float dist = distance(0, pos);
    if (dist < near)
        return 0;
    
    return (min(dist, far) - near) / (far - near);
}