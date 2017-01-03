#pragma once

#include "wallList.h"
#include "ofxWinTouchHook.h"

class wallMgr
{
public:
	wallMgr();
	~wallMgr();

	void setup(ePhotoPrimaryCategory category, ofRectangle wallRect);
	void update(float delta);
	void draw(ofVec2f pos);

	void addWallList(int width);
	
private:
	void setupCheck();

private:
	bool	_isSetup;
	ofRectangle	_wallRect;
	ePhotoPrimaryCategory	_eCategory;

	vector<ofPtr<wallList>> _wallListMgr;
	ofPtr<wallList> _selectWallList;

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