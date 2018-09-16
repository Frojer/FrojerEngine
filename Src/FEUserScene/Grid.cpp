#include "Grid.h"
#include <FEDebug.h>

void Grid::Update()
{
	///*
#define GRID_SIZE 50.0f
#define GRID_WIDTH 1.0f
	for (int i = 0; i <= GRID_SIZE; i++)
	{
		if (x)
		{
			// x축 그리드
			FEDebug::GetInstance()->DrawLine(FEVector3(0, -(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector3(0, GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector4(0.3f, 0.3f, 0.3f, 1));
			FEDebug::GetInstance()->DrawLine(FEVector3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2)), FEVector3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2)), FEVector4(0.3f, 0.3f, 0.3f, 1));
		}

		if (y)
		{
			// y축 그리드
			FEDebug::GetInstance()->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2), 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector3(GRID_SIZE * GRID_WIDTH / 2, 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector4(0.3f, 0.3f, 0.3f, 1));
			FEDebug::GetInstance()->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, -(GRID_SIZE * GRID_WIDTH / 2)), FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, (GRID_SIZE * GRID_WIDTH / 2)), FEVector4(0.3f, 0.3f, 0.3f, 1));
		}

		if (z)
		{
			// z축 그리드
			FEDebug::GetInstance()->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), FEVector3(GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), FEVector4(0.3f, 0.3f, 0.3f, 1));
			FEDebug::GetInstance()->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2), 0), FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2), 0), FEVector4(0.3f, 0.3f, 0.3f, 1));
		}
	}
	//*/
}