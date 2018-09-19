#pragma once
#ifndef _FE_MATERIAL
#define _FE_MATERIAL

#include <FEMath.h>
#include "FEObjectHeader.h"

#define FE_LIGHT_SIZE 5

class FEShader;
class FETransform;
enum COMPARISON_FUNC;

enum ADDRESS_MODE
{
	ADDRESS_MODE_WRAP,
	ADDRESS_MODE_MIRROR,
	ADDRESS_MODE_CLAMP,
	ADDRESS_MODE_BORDER,
	ADDRESS_MODE_MIRROR_ONCE
};

class FEMaterial : public FEObject
{
private:
	struct TexInfo
	{
		FEVector4 ot;
		FEVector4 angle_Amount;
		
		FETexture* pTexture;
		TexInfo() : ot(0.0f, 0.0f, 1.0f, 1.0f), angle_Amount(0.0f, 0.0f, 0.0f, 1.0f), pTexture(nullptr) {}
	};
	struct LightCB
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
	struct PerCameraCB
	{
		FEMatrixA mView;
		FEMatrixA mProj;
	};
	struct PerMaterialCB
	{
		FEVectorA diffuse;
		FEVectorA ambient;
		FEVectorA specular;
		float power;
	};
	struct PerObjectCB
	{
		FEMatrixA mPos;
		FEMatrixA mRot;
		FEMatrixA mScale;
		FEMatrixA mRP;
		FEMatrixA mWorld;
		FEMatrixA mWV;
		FEMatrixA mWVNormal;
		FEMatrixA mWVP;
		FEVectorA vLightLocalPos[FE_LIGHT_SIZE];
		FEVectorA vLightLocalDir[FE_LIGHT_SIZE];
	};

	static LightCB _lightCB[FE_LIGHT_SIZE];
	static PerCameraCB _perCamCB;
	static PerMaterialCB _perMtrlCB;
	static PerObjectCB _perObjCB;
	static INT64 _oldDrawID;

	static std::unordered_map<INT64, FEMaterial*> _mtrlMap;
	static FETexture* _pDefaultTex;

	FEShader* _pShader;

	std::vector<FEVectorA> _constData;

	std::vector<TexInfo> _vecTexInfo;
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
	static void UpdateConstantBufferLight();
	static void UpdateConstantBufferPerCamera(const FEMatrixA& mView, const FEMatrixA& mProj);
	void UpdateConstantBufferPerMaterial();
	void UpdateConstantBufferPerObject(FETransform* tr);
	void UpdateConstantBuffer(FETransform* tr);
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

	void SetSamplerState(const UINT StartSlot, FE_SAMPLER_STATE_FLAG& sampler);

	static FEMaterial* Find(INT64 id);
	static FEMaterial* Find(LPCTSTR name);

	friend class FELoader;
	friend class IFEScene;
	friend class FEShader;
	friend class FERenderer;
};

#endif