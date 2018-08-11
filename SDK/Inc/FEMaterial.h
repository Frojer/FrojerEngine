//#pragma once
//#ifndef _FE_MATERIAL
//#define _FE_MATERIAL
//
//#include <FEMath.h>
//#include "FEObjectHeader.h"
//
//#define LIGHT_SIZE 5
//
//class FEMaterial : public FEObject
//{
//private:
//	struct WVP_Data
//	{
//		FEMath::FEMatrixA mWorld;
//		FEMath::FEMatrixA mView;
//		FEMath::FEMatrixA mWV;
//		FEMath::FEMatrixA mProj;
//	};
//
//	struct Light_Data
//	{
//		float range;
//		UINT lightType;
//		bool useLight;
//		FEMath::FEVectorA diffuse;
//		FEMath::FEVectorA ambient;
//		FEMath::FEVectorA specular;
//		FEMath::FEVectorA position;
//		FEMath::FEVectorA direction;
//	};
//
//	static std::unordered_map<UINT, FEMaterial*> _mtrlMap;
//	static FETexture* _pDefaultTex;
//
//	FEShader* _pShader;
//	UINT _countTexture;
//	static WVP_Data _WVPData;
//	static Light_Data _LightData[LIGHT_SIZE];
//	std::vector<FEMath::FEVectorA> _constData;
//
//	vector<FEVector4>	_vecScalar;
//	vector<FEMath::FEVectorA>	_vecScalarA;
//	vector<FEMath::FEVectorA>	_vecVector;
//	vector<FEMath::FEMatrixA>	_vecMatrix;
//
//public:
//	FEVector4 m_diffuse;
//	FEVector3 m_ambient;
//	FEVector3 m_specular;
//
//	FETexture* m_pTexture[128];
//
//private:
//	static void ClearMap();
//	static void UpdateLightData();
//	void UpdateConstantBuffer(FEMath::FEMatrixA& mWorld);
//	void Render();
//
//public:
//	FEMaterial(FEShader* shader);
//	virtual ~FEMaterial();
//
//	void SetShader(FEShader* shader);
//	FEShader* GetShader();
//
//	void SetScalar(UINT id, const float scalar);
//	void SetScalar(UINT id, const int scalar);
//	void SetScalar(UINT id, const UINT scalar);
//	void SetScalar(UINT id, const bool scalar);
//	void SetVector(UINT id, const FEVector4& vector);
//	void SetMatrix(UINT id, const FEMatrix& matrix);
//
//	static FEMaterial* Find(UINT id);
//	static FEMaterial* Find(LPCTSTR name);
//};
//
//#endif