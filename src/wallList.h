#pragma once

#include "wallUnit.h"
#include "dataHolder.h"
#include "imageRender.h"
#include "inputEventMgr.h"

#include "ofxAnimatableFloat.h"
class wallMgr;
class wallList : public inputEvent
{
public:
	wallList(wallMgr* parent, ePhotoPrimaryCategory eCategroy, ofRectangle drawArea);
	~wallList();

	void update(float delta);
	void draw();
	void draw(ofVec2f pos);
	float getBaseWidth();
	
	int getListPosX();
	
private:
	ePhotoPrimaryCategory _eCategroy;
	ofRectangle	_drawArea;
	wallMgr*	_parent;
#pragma region Center
//Center
private:
	void resetCenter();
	void updateCenter(float delta);
	int touchCheckUp(float diff, ofVec2f& pos);
	int touchCheckDown(float diff, ofVec2f& pos);

	int getCenterUnitPosYFromSelect();

private:
	ofVec2f _centerVec, _centerAcc;
	ofVec2f _centerUnitPos, _centerUnitPosBackup;
	
#pragma endregion

#pragma region animation
public:
	bool getIsSelect();
	bool select(ofVec2f& pos);
	bool deselect();

private:
	void setupAnimation(int posX, int width);
	void updateAnimation(float delta);
	
	void checkAnimationState();
	
	int getAnimMoveX();

private:
	enum selectState {
		eDeselect = 0
		,eZoomIn
		,eSelect
		,eZoomOut
	}_eState;

	bool _selectUp;
	int _selectId;
	ofVec2f _selectPos;
	ofxAnimatableFloat	_animDrawPosX, _animDrawWidth;
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
private:
	void setupInput();
	void inputPress(ofVec2f pos) override;
	void inputDrag(ofVec2f delta) override;
	void inputRelease(ofVec2f pos) override;
	ofRectangle	getInputArea() override; 
#pragma endregion


};