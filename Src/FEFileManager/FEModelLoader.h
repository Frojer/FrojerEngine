#pragma once

#include "ModelLoader.h"

class FEModelLoader	: public ModelLoader
{
private:
	FEGameObject* currentObject;
	FEGameObject* beforeObject;

	int numVertex, numFaces, numTVertex, numTVFaces, numCVertex, numCVFaces;
	UINT64 indexBeginNum;

	static FEModelLoader* _pInstance;

public:
	~FEModelLoader();

	FEObject* LoadModel(TCHAR* file);

	static FEModelLoader* GetInstance();

protected:
	virtual void CreateModel();
	// ���ο� �𵨷� �а� �ɰ�� ȣ�� (pParentName�� ���̸� �ٲ�� ���� ���� �θ�)
	virtual void SetParent(TCHAR* pParentName);
	virtual void LoadModelName(TCHAR* pName);
	virtual void LoadPosition(float x, float y, float z);
	virtual void LoadRotation(float x, float y, float z);
	virtual void LoadScale(float x, float y, float z);
	virtual void LoadTexture(TCHAR* pPath);
	virtual void SetMaterial(UINT i);
	virtual void FinishLoadModel();

private:
	FEModelLoader();
	void VertexWeld(FEMesh* pMesh);
};