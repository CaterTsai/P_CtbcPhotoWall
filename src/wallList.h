#pragma once

#include "wallUnit.h"
#include "dataHolder.h"
#include "imageRender.h"
#include "inputEventMgr.h"

#include "ofxAnimatableFloat.h"
class wallMgr;

struct wallUnitInfo
{
	wallUnitInfo()
		:id(-1)
		,pos(0)
	{}
	int id;
	ofVec2f pos;
};

class wallList : public inputEvent
{
public:
	wallList(wallMgr* parent, ePhotoPrimaryCategory eCategroy, ofRectangle drawArea);
	~wallList();

	void update(float delta);
	void draw();
	float getBaseWidth();
	
	int getListPosX();
	
private:
	ePhotoPrimaryCategory _eCategroy;
	ofRectangle	_baseArea;
	wallMgr*	_parent;
#pragma region Center
//Center
private:
	void resetCenter();
	void updateCenter(float delta);
	void updateSelectCenter(float delta);
	
	wallUnitInfo foundWallUnit(ofVec2f pos, bool nearest = false);
	wallUnitInfo foundUnitUp(float diff);
	wallUnitInfo foundUnitDown(float diff);
	wallUnitInfo foundUnitUpNearest(float diff);
	wallUnitInfo foundUnitDownNearest(float diff);

	int getCenterUnitPosYFromSelect();
	void fixCenterUnitPos();
	
private:
	ofVec2f _centerVec, _centerBaseVec;
	ofVec2f _centerUnitPos;
	
#pragma endregion

#pragma region animation
public:
	bool getIsSelect();
	bool select(ofVec2f& pos);
	bool deselect();

private:
	void setupAnimation(int posX, int width);
	void updateAnimation(float delta);
	
	void checkSelectState();
	void checkSelectDrapState();
	int getAnimMoveX();
	void fitSelectPos();
private:
	enum selectState {
		eDeselect = 0
		,eZoomIn
		,eSelect
		,eZoomOut
	}_eSelectState;

	enum selectDrapState {
		eStable	=	0
		,eDrap
		,eMove
	}_eSelectDrapState;

	wallUnitInfo _selectWallUnit;
	ofxAnimatableFloat _animDrawPosX, _animDrawWidth;
	ofxAnimatableFloat _animMoveSelect;
#pragma endregion
	
#pragma region WallUnit
	//WallUnit
private:
	void setupWallUnit();
	void updateWallUnit(float delta);
	void drawWallUnitUp();
	void drawWallUnitDown();

	void addWallUnit(ofPtr<wallUnit> newUnil);
	void addWallUnits();
	void addWallUnits(int type);

	void removeWallUnits();

	void updateWallTotalHeight();
private:
	int _wallTotalHeight;
	vector<ofPtr<wallUnit>>	_wallUnitList;
#pragma endregion

#pragma region Input
public:
	void enableInput(bool isSelect = false);
	void disableInput();
private:
	void inputPress(inputEventArgs e) override;
	void inputDrag(inputEventArgs e) override;
	void inputRelease(inputEventArgs e) override;
	ofRectangle	getInputArea() override; 
#pragma endregion


};