#pragma once

#include "wallList.h"
#include "mainUI.h"

#include "ofxWinTouchHook.h"

class wallMgr : public inputEvent
{
public:
	wallMgr();
	~wallMgr();

	void setup(ePhotoPrimaryCategory category, ofRectangle wallRect);
	void update(float delta);
	void draw();
	void drawSelect();
	void drawShadow();

	void addWallList(int width);
	int getWallRectWidth();

private:
	void setupCheck();
	int getListTotalWidth();

	void enableWallListInput();
	void disableWallListInput();
	
private:
	enum eWallState
	{
		eWallIdle	=	0
		,eWallMainUI
		,eWallPhoto
	}_eWallState;
	bool	_isSetup;
	ofRectangle	_wallRect;
	ePhotoPrimaryCategory	_eCategory;
	vector<ofPtr<wallList>> _wallListMgr;

#pragma region UI
public:
	void mainUIin();
	void mainUIout();
private:
	void drawUI();
private:
	mainUI	_mainUI;
#pragma endregion
	
#pragma region Select
public:
	void selectCheck(wallList* selectList);


private:
	wallList* _selectWallList;
#pragma endregion

#pragma region Input
private:
	void enableInput();
	void disableInput();
	void inputPress(ofVec2f pos) override;
	ofRectangle	getInputArea() override;
#pragma endregion



};