#pragma once
#ifndef _FE_MATERIAL
#define _FE_MATERIAL

#include <FEMath.h>
#include "FEObjectHeader.h"

#define FE_LIGHT_SIZE 5
#define FE_TEXTURE_SIZE 16

class FEShader;

class FEMaterial : public FEObject
{
private:
	struct TEX_INFO
	{
		FEVector4 ot;
		FEVector4 angle_Amount;
		FETexture* pTexture;
		TEX_INFO() : ot(0.0f, 0.0f, 1.0f, 1.0f), angle_Amount(0.0f, 0.0f, 0.0f, 1.0f), pTexture(nullptr) {}
	};
	struct WVP_Data
	{
		FEMatrixA mWorld;
		FEMatrixA mView;
		FEMatrixA mWV;
		FEMatrixA mProj;
	};
	struct Light_Data
	{
		FEVectorA diffuse;
		FEVectorA ambient;
		FEVectorA specular;
		FEVectorA position;
		FEVectorA direction;
		float range;
		UINT lightType;
		bool useLight;
	};
	struct Texture_Data
	{
		FEVectorA ot;
		FEVectorA angle_Amount;
		FETexture* pTexture;
	};

	static std::unordered_map<INT64, FEMaterial*> _mtrlMap;
	static FETexture* _pDefaultTex;

	static WVP_Data _WVPData;
	static Light_Data _LightData[FE_LIGHT_SIZE];
	static Texture_Data _TexturetData[FE_TEXTURE_SIZE];

	FEShader* _pShader;
	UINT _countTexture;
	TEX_INFO _texInfo[FE_TEXTURE_SIZE];

	std::vector<FEVectorA> _constData;

	std::vector<FEVector4>	_vecScalar;
	std::vector<FEVectorA>	_vecScalarA;
	std::vector<FEVectorA>	_vecVector;
	std::vector<FEMatrixA>	_vecMatrix;

public:
	FEVector4 m_diffuse;
	FEVector3 m_ambient;
	FEVector3 m_specular;
	float m_power;

private:
	static void ClearMap();
	static void UpdateLightData();
	void UpdateConstantBuffer(FEMatrixA& mWorld);
	void Render();

protected:
	FEMaterial(INT64 ID, FEShader* i_pShader);

public:
	FEMaterial(FEShader* i_pShader);
	virtual ~FEMaterial();

	void SetShader(FEShader* i_pShader);
	FEShader* GetShader() const;

	void SetScalar(const UINT index, const float scalar);
	void SetScalar(const UINT index, const int scalar);
	void SetScalar(const UINT index, const UINT scalar);
	void SetScalar(const UINT index, const bool scalar);
	void SetVector(const UINT index, const FEVector4& vector);
	void SetMatrix(const UINT index, const FEMatrix& matrix);

	bool GetTexture(const UINT index, FETexture*& o_texture) const;
	bool SetTexture(const UINT index, FETexture* i_texture);
	bool GetTextureOffset(const UINT index, FEVector2& o_offset) const;
	bool SetTextureOffset(const UINT index, const FEVector2 i_offset);
	bool GetTextureTiling(const UINT index, FEVector2& o_tiling) const;
	bool SetTextureTiling(const UINT index, const FEVector2 i_tiling);
	bool GetTextureAngle(const UINT index, FEVector3& o_angle) const;
	bool SetTextureAngle(const UINT index, const FEVector3 i_angle);
	bool GetTextureAmount(const UINT index, float& o_amount) const;
	bool SetTextureAmount(const UINT index, const float i_amount);

	static FEMaterial* Find(INT64 id);
	static FEMaterial* Find(LPCTSTR name);

	friend class FELoader;
	friend class IFEScene;
	friend class FEShader;
	friend class FERenderer;
};

#endif