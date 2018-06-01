#pragma once

class FEWindow
{
public:
	unsigned int width, height;
public:
	FEWindow();
	virtual ~FEWindow();


};

bool CreateFEWindow();