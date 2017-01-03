#pragma once

#include "wallUnit.h"
#include "dataHolder.h"
#include "ofxAnimatableFloat.h"

class wallList
{
public:
	wallList(ePhotoPrimaryCategory eCategroy, int width, int height);
	~wallList();

	void update(float delta);
	void draw(ofVec2f pos);
	float getBaseWidth();
	
	
private:
	ePhotoPrimaryCategory _eCategroy;
	
	int _baseWidth;
	const int _displayheight;

#pragma region Center
//Center
public:
	void touchDown(ofVec2f& pos);
	void touchMoved(ofVec2f& delta);
	void touchUp(ofVec2f& pos);

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
	void setupAnimation(int width);
	void updateAnimation(float delta);
	void checkAnimationState();
	
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
	ofxAnimatableFloat	_animDrawWidth;
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




};