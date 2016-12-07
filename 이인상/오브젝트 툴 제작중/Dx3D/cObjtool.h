#pragma once
#include "cObjectXfile.h"
#include <fstream>

enum OBJNUM
{
	rock
};

class cObjtool
{
private:
	OBJNUM Objnum = rock;

	cObjectXfile* o_Object;

	float Xplus;
	float Yplus;
	float Zplus;

	float scaling;

	float Rotate;
	float Rotateplus;

	cObjectXfile* o_Objectswap;
	vector<cObjectXfile*> o_Image;
public:
	void Setup();
	void Update();
	void Render();

	void Pasing();

	void ObjMove();
	void ObjSetting();

	void OutputSave();
	cObjtool();
	~cObjtool();
};

