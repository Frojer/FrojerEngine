#pragma once

#include <vector>
#include <FEObjectHeader.h>

struct ML_INDEX
{
	int a, b, c;
};

class ModelLoader
{
protected:
	std::vector<FEVector3>	vPos;
	std::vector<ML_INDEX>	iPos;
	std::vector<FEVector4>	vColor;
	std::vector<ML_INDEX>	iColor;
	std::vector<FEVector3>	vVertexNormal;
	std::vector<FEVector3>	vFaceNormal;
	std::vector<FEVector2>	vTex;
	std::vector<ML_INDEX>	iTex;

	std::vector<FEMaterial*> pMtrls;

public:
	ModelLoader();
	~ModelLoader();

	void ASEFileParce(const TCHAR* fileName);

protected:
	virtual void CreateModel() = 0;
	// 새로운 모델로 읽게 될경우 호출 (pParentName이 널이면 바뀌기 전의 모델이 부모)
	virtual void SetParent(TCHAR* pParentName) = 0;
	virtual void LoadModelName(TCHAR* pName) = 0;
	virtual void LoadPosition(float x, float y, float z) = 0;
	virtual void LoadRotation(float x, float y, float z) = 0;
	virtual void LoadScale(float x, float y, float z) = 0;
	virtual void LoadTexture(TCHAR* pPath) = 0;
	virtual void SetMaterial(UINT i) = 0;
	virtual void FinishLoadModel() = 0;
};