#pragma once
#ifndef _FE_SHADER
#define _FE_SHADER

#include <IFEShader.h>
#include <IFEBuffer.h>
#include "FEObjectHeader.h"

class FEShader : public FEObject
{
private:
	static std::unordered_map<INT64, FEShader*> _shaderMap;

	static IFEBuffer* _pWVP_CB;
	static IFEBuffer* _pLight_CB;

	IFEBuffer* _pConstBuffer;
	IFEShader* _pShader;

	FE_SHADER_SEMANTICS _semantics;

	UINT _countTexture;
	UINT _countMatrix;
	UINT _countVector;
	UINT _countScalar;

	bool _useLight;

private:
	static void ClearMap();

	static bool CreateDefaultConstantBuffer();
	static void ReleaseDefaultConstantBuffer();

	bool CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName, FE_SHADER_SEMANTICS i_semantics);

	bool CreateConstantBuffer();
	void UpdateConstantBuffer(const void* pCB, UINT size);

protected:
	FEShader(INT64 ID);

public:
	FEShader();
	virtual ~FEShader();

	void Release();

	void Render() const;

	static FEShader* Find(INT64 id);
	static FEShader* Find(LPCTSTR name);

	friend class FELoader;
	friend class FESystem;
	friend class FEMaterial;
};

#endif