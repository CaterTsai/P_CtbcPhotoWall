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
	int getBasePosX();
	int getDrawPosX();
	
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
	wallUnitInfo foundWallUnit(int index);
	wallUnitInfo foundUnitUp(float diff);
	wallUnitInfo foundUnitDown(float diff);
	wallUnitInfo foundUnitUpNearest(float diff);
	wallUnitInfo foundUnitDownNearest(float diff);

	int getCenterUnitPosYFromSelect();
	void fixCenterUnitPos();
	void fixCenterUnitPosByUnit(int idx, int posY);
	
private:
	ofVec2f _centerVec, _centerBaseVec;
	ofVec2f _centerUnitPos;
	
#pragma endregion

#pragma region animation
public:
	bool getIsSelect();
	bool getIsDeselect();
	bool select(ofVec2f& pos);
	bool deselect();

	int getSelectPhotoID();
	int getSelectTopPosY();
	int getSelectBottomPosY();
private:
	void setupAnimation(int posX, int width);
	void updateAnimation(float delta);
	
	void movePosY(int posY, float t);

	void checkSelectState();
	void checkMoveCenterYState();
	int getAnimMoveX();
	void fitSelectPos();
	void moveSelectPos(wallUnitInfo newSelectUnit);
	
private:
	enum selectState {
		eDeselect = 0
		,eZoomIn
		,eSelect
		,eZoomOut
	}_eSelectState;

	enum moveCenterYState {
		eStable	=	0
		,eMove
	}_eMoveCenterYState;

	wallUnitInfo _selectWallUnit;
	ofxAnimatableFloat _animDrawPosX, _animDrawWidth;
	ofxAnimatableFloat _animDrawPosY;
#pragma endregion
	
#pragma region WallUnit
//WallUnit
public:
	void selectType(PHOTO_TYPE type);
	void changeCategory(ePhotoPrimaryCategory category);
private:
	void setupWallUnit();
	void updateWallUnit(float delta);
	void drawWallUnitUp();
	void drawWallUnitDown();

	void addWallUnit(ofPtr<wallUnit> newUnil);
	void addWallUnit(int index, ofPtr<wallUnit> newUnil);
	void resetWallUnits();
	int insertWallUnits(int index, PHOTO_TYPE type);
	int insertWallUnits(int index);
	void removeWallUnits(int start, int end);
	void clearWallUnits();

	void findDisplayRange(wallUnitInfo& start, wallUnitInfo& end);

	void updateWallTotalHeight();
private:
	bool _isInsert;
	int _wallTotalHeight;
	vector<ofPtr<wallUnit>>	_wallUnitList;
	int _insertStart, _insertEnd;
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