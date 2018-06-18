//#pragma once
//#ifndef _FE_MATERIAL
//#define _FE_MATERIAL
//
//#include <FEMath.h>
//#include "FEObjectHeader.h"
//
//#define LIGHT_SIZE 5
//
//class FEMaterial : public IFEObject
//{
//private:
//	struct WVP_Data
//	{
//		MATRIXA mWorld;
//		MATRIXA mView;
//		MATRIXA mWV;
//		MATRIXA mProj;
//	};
//
//	struct Light_Data
//	{
//		float range;
//		UINT lightType;
//		bool useLight;
//		VECTOR diffuse;
//		VECTOR ambient;
//		VECTOR specular;
//		VECTOR position;
//		VECTOR direction;
//	};
//
//	static std::unordered_map<UINT, FEMaterial*> _mtrlMap;
//	static CTexture2D* _pDefaultTex;
//
//	CShader* _pShader;
//	UINT _countTexture;
//	static WVP_Data _WVPData;
//	static Light_Data _LightData[LIGHT_SIZE];
//	std::vector<VECTOR> _constData;
//
//	vector<VECTOR4>	_vecScalar;
//	vector<VECTOR>	_vecScalarA;
//	vector<VECTOR>	_vecVector;
//	vector<MATRIXA>	_vecMatrix;
//
//public:
//	VECTOR4 m_diffuse;
//	VECTOR3 m_ambient;
//	VECTOR3 m_specular;
//
//	CTexture2D* m_pTexture[128];
//
//private:
//	static void ClearMap();
//	static void UpdateLightData();
//	void UpdateConstantBuffer(MATRIXA& mWorld);
//	void Render();
//
//public:
//	FEMaterial(CShader* shader);
//	virtual ~FEMaterial();
//
//	void SetShader(CShader* shader);
//	CShader* GetShader();
//
//	void SetScalar(UINT id, const float scalar);
//	void SetScalar(UINT id, const int scalar);
//	void SetScalar(UINT id, const UINT scalar);
//	void SetScalar(UINT id, const bool scalar);
//	void SetVector(UINT id, const VECTOR4& vector);
//	void SetMatrix(UINT id, const MATRIX& matrix);
//
//	static FEMaterial* Find(UINT id);
//	static FEMaterial* Find(LPCWSTR name);
//};
//
//#endif