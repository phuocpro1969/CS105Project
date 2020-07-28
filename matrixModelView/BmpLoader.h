#pragma once
#include<windows.h>
#include<iostream>
class BmpLoader
{
public:
	unsigned char* textureData;
	int iWidth, iHeight;
	BmpLoader(const char*);
	
	~BmpLoader();
private:
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
};

