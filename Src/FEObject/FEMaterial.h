#pragma once
#ifndef _FE_MATERIAL
#define _FE_MATERIAL

#include <FEMath.h>
#include "FEObjectHeader.h"

#define LIGHT_SIZE 5

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
		float range;
		UINT lightType;
		bool useLight;
		FEVectorA diffuse;
		FEVectorA ambient;
		FEVectorA specular;
		FEVectorA position;
		FEVectorA direction;
	};

	static std::unordered_map<UINT, FEMaterial*> _mtrlMap;
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

	FETexture* m_pTexture[128];

private:
	FEMaterial(FEShader* i_pShader);
	static void ClearMap();
	static void UpdateLightData();
	void UpdateConstantBuffer(FEMatrixA& mWorld);
	void Render();

public:
	virtual ~FEMaterial();

	static FEMaterial* CreateMaterial(FEShader* i_pShader);

	void SetShader(FEShader* i_pShader);
	FEShader* GetShader();

	void SetScalar(UINT id, const float scalar);
	void SetScalar(UINT id, const int scalar);
	void SetScalar(UINT id, const UINT scalar);
	void SetScalar(UINT id, const bool scalar);
	void SetVector(UINT id, const FEVector4& vector);
	void SetMatrix(UINT id, const FEMatrix& matrix);

	static FEMaterial* Find(UINT id);
	static FEMaterial* Find(LPCTSTR name);

	friend class FEShader;
	friend class FERenderer;
};

#endif