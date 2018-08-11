#include "FEObjectHeader.h"

FEShader::FEShader()
	: _pShader(nullptr)
{
}


FEShader::~FEShader()
{
	Release();
}


void FEShader::Release()
{
	_pShader->Release();
}


void FEShader::Render()
{
	_pShader->Render();
}