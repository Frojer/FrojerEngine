#pragma once
#ifndef _FE_SHADER
#define _FE_SHADER

#include <IFEShader.h>
#include <IFEBuffer.h>
#include "FEObjectHeader.h"

class FEShader : public FEObject
{
private:
	static std::unordered_map<UINT, FEShader*> _shaderMap;

	static IFEBuffer* _pWVP_CB;
	static IFEBuffer* _pLight_CB;

	IFEBuffer* _pConstBuffer;
	IFEShader* _pShader;

	UINT _countTexture;
	UINT _countMatrix;
	UINT _countVector;
	UINT _countScalar;

	bool _useLight;

private:
	FEShader();

	static void ClearMap();

	static bool CreateDefaultConstantBuffer();
	static void ReleaseDefaultConstantBuffer();

	bool CreateConstantBuffer();
	void UpdateConstantBuffer(const void* pCB, UINT size);

public:
	virtual ~FEShader();

	static FEShader* CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName);

	void Release();

	void Render() const;

	static FEShader* Find(UINT id);
	static FEShader* Find(LPCTSTR name);

	friend void LoadShader(tstring i_path);
	friend class FESystem;
	friend class FEMaterial;
};

#endif