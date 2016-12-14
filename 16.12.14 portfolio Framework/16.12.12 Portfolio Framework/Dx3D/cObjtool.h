#pragma once
#include "cObjectXfile.h"
#include <fstream>

enum OBJECT
{
	rock1, rock2, rock3, rock4, rock5, rock6, rock7, rock8, rock9, rock10,
	rock11, tree1, tree2, tree3, tree4, tree5, mush1, mush2, mush3, mush4,
	mush5, mush6, mush7, mush8, mush9, mush10, flow1, flow2, flow3, flow4,
	flow5, flow6, flow7, flow8, flow9, flow10, flow11, furn1, furn2, furn3,
	tooth, furn5, furn6, furn7, furn8, furn9, furn10, furn11, furn12, furn13,
	etc1, etc2, etc3, etc4, etc5, etc6, etc7, etc8, etc9, etc10

};

class cObjtool
{
private:
	int Objnumtab = 0;
	OBJECT Objnum = rock1;
	int PNum = 0;

	cObjectXfile* o_Object;

	float Xplus;
	float Yplus;
	float Zplus;

	float scaling;

	float RotationX;
	float RotationY;
	float RotationZ;

	cObjectXfile* o_Objectswap;
	vector<cObjectXfile*> o_Image;
public:
	void Setup();
	void Update();
	void Render();

	void ObjPasing();
	void ObjSelect();
	cObjtool();
	~cObjtool();
};

