#pragma once

#include "constParameter.h"
#include "fontMgr.h"
#include "inputEventMgr.h"
#include "dataHolder.h"
#include "ofxAnimatableFloat.h"

class wallMgr;
class textUI : public inputEvent
{
public:
	textUI();
	void setup(wallMgr* parent);
	void update(float delta);
	void draw(ofVec2f pos);

	void updateText(string& title, string& context, bool isZH);
	void open();
	void close();

private:
	void updateCanvas();
	void animStateCheck();
	bool moveTextDrawY(int delta);
	string reconstructMsg(int widthLimit, string& msg, vector<int>& eachWordWidth);


private:
	enum eAnimState
	{
		eClose = 0
		,eUIIn
		,eOpen
		,eUIOut
	}_eState;
	bool _isSetup;
	
	wallMgr*	_parent;
	ofFbo	_displayCanvas, _textCanvas;
	int _textDrawYMin, _textDrawY;
	ofxAnimatableFloat	_animHeight;

#pragma region Input
public:
	void enableInput(bool isSelect = false);
	void disableInput();
private:
	void inputDrag(inputEventArgs e) override;
	ofRectangle	getInputArea() override;
#pragma endregion
};