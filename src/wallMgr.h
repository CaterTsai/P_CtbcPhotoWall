#pragma once

#include "wallList.h"
#include "mainUI.h"

#include "ofxWinTouchHook.h"

class wallMgr
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
	wallList* _selectWallList;
#pragma endregion

#pragma region Input
#ifdef USE_MOUSE
public:
	void mouseDragged(ofMouseEventArgs& e);
	void mousePressed(ofMouseEventArgs& e);
	void mouseReleased(ofMouseEventArgs& e);
	void mouseMoved(ofMouseEventArgs& e) {};
	void mouseScrolled(ofMouseEventArgs& e) {};
	void mouseEntered(ofMouseEventArgs& e) {};
	void mouseExited(ofMouseEventArgs& e) {};
	
#else
public:
	void touchDown(ofTouchEventArgs& e);
	void touchMoved(ofTouchEventArgs& e);
	void touchUp(ofTouchEventArgs& e);
#endif // USE_MOUSE
	
public:
	void enableInput();
	void disableInput();
#pragma endregion

};