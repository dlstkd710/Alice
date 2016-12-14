#pragma once

class iMap : public cObject
{
public:
	iMap(void);
	virtual ~iMap(void);

	virtual float GetHeight2(IN float x, IN float z) = 0;

	virtual void Render() = 0;
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;

};

