#pragma once
#ifndef _FE_MATERIAL
#define _FE_MATERIAL

#include <FEMath.h>
#include "FEObjectHeader.h"

#define LIGHT_SIZE 5

class FEShader;

class FEMaterial : public FEObject
{
private:
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

	static std::unordered_map<INT64, FEMaterial*> _mtrlMap;
	static FETexture* _pDefaultTex;

	FEShader* _pShader;
	UINT _countTexture;
	static WVP_Data _WVPData;
	static Light_Data _LightData[LIGHT_SIZE];
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

	FETexture* m_pTexture[128];

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

	void SetScalar(UINT id, const float scalar);
	void SetScalar(UINT id, const int scalar);
	void SetScalar(UINT id, const UINT scalar);
	void SetScalar(UINT id, const bool scalar);
	void SetVector(UINT id, const FEVector4& vector);
	void SetMatrix(UINT id, const FEMatrix& matrix);

	static FEMaterial* Find(INT64 id);
	static FEMaterial* Find(LPCTSTR name);

	friend class FELoader;
	friend class IFEScene;
	friend class FEShader;
	friend class FERenderer;
};

#endif