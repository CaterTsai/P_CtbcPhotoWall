#pragma once

#include "constParameter.h"
#include "wallUnit.h"
#include "renderMgr.h"

class wallUnit
{
public:
	wallUnit(eWallUnitType unitType)
		:_unitType(unitType)
		, _isClick(false)
	{};
	~wallUnit() {};

	virtual void update(float delta, int width) {};
	virtual void draw(ofVec2f pos) {};
	virtual void onclick(ofVec2f pos) {};
	

	virtual eWallUnitType getUnitType() { return _unitType; };

	virtual int getHeight() { return _height; };
	virtual int getHeight(int width) { return _height;  }

protected:
	bool _isClick;
	int _width, _height;
	eWallUnitType _unitType;
	
};


//--------------------------------------
//Photo 
class photoUnit :public wallUnit
{
public:
	photoUnit(stPhotoHeader& photoIdx, int width);

	void update(float delta, int width) override;
	void draw(ofVec2f pos) override;
	void onclick(ofVec2f pos) override;
	int getHeight(int width) override;
	stPhotoHeader& getPhotoHeader();
private:
	void setRatio();
protected:
	float _photoRatio;
	
	stPhotoHeader	_photoIndex;
};
