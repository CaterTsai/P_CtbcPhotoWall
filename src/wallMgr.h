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


#pragma region Select
private:
	void setupSelect();
	void updateSelect(float delta);
	void drawSelect();
	void select(ofVec2f inputP, ofPtr<wallList>& selectList, ofVec2f pos);
	void deselect();
	void selectAnimationCheck();

private:
	enum eSelectState {
		eDeselect = 0
		,eMoveFront
		,eSelect
		,eMoveBack
	}_eState;
	ofVec2f _selectPosBackup;
	ofxAnimatableFloat	_animSelectPosX;
	ofPtr<wallList> _selectWallList;

	std::function<void(void)> _afterSelect;
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