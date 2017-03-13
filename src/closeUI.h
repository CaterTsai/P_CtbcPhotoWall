#pragma once

#include "constParameter.h"
#include "inputEventMgr.h"
#include "ofxAnimatableFloat.h"
#include "imageRender.h"
class wallMgr;
class closeUI : public inputEvent
{
public:
	closeUI();
	void setup(wallMgr* parent);
	void draw(ofVec2f pos);
	void setDisplay(bool val);

private:
	bool _isDisplay;
	wallMgr*	_parent;

#pragma region Input
public:
	void enableInput();
	void disableInput();
private:
	void inputRelease(inputEventArgs e) override;
	ofRectangle	getInputArea() override;
#pragma endregion
};