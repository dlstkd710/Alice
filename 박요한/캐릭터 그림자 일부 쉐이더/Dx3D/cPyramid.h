#pragma once
class cPyramid
{
private:
	std::vector<ST_PC_VERTEX> m_vecVertex;

public:
	cPyramid(void);
	~cPyramid(void);

	void Setup(D3DCOLOR c, D3DXMATRIXA16* pmat = NULL);
	void Render();
};

