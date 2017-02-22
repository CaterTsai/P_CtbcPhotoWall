#include "closeUI.h"
#include "wallMgr.h"
//--------------------------------------------------------------
closeUI::closeUI()
	:_parent(nullptr)
	, _isDisplay(false)
{
}

//--------------------------------------------------------------
void closeUI::setup(wallMgr * parent)
{
	_parent = parent;
}

//--------------------------------------------------------------
void closeUI::draw(ofVec2f pos)
{
	if (!_isDisplay)
	{
		return;
	}

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos);
	{
		ofSetColor(cCloseUIBGColor);
		ofCircle(0, 0, cCloseUIBGRadius);

		ofSetColor(cCloseUIColor);
		ofSetLineWidth(cCloseUIWidth);
		
		ofPushMatrix();
		ofRotateZ(45);
		{
			ofLine(0, cCloseUILength * -0.5, 0, cCloseUILength * 0.5);
		}
		ofPopMatrix();

		ofPushMatrix();
		ofRotateZ(-45);
		{
			ofLine(0, cCloseUILength * -0.5, 0, cCloseUILength * 0.5);
		}	
		ofPopMatrix();

	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void closeUI::setDisplay(bool val)
{
	_isDisplay = val;
}

#pragma region Input
//--------------------------------------------------------------
void closeUI::enableInput()
{
	inputEventMgr::GetInstance()->registerInputEvent(this, eInputCloseUI);
}

//--------------------------------------------------------------
void closeUI::disableInput()
{
	inputEventMgr::GetInstance()->unregisterInputEvent(this);
}

//--------------------------------------------------------------
void closeUI::inputRelease(inputEventArgs e)
{
	if (_parent != nullptr)
	{
		_parent->closeSelect();
	}
}

//--------------------------------------------------------------
ofRectangle closeUI::getInputArea()
{
	ofRectangle rVal_;
	if (_parent != nullptr && _isDisplay)
	{
		auto drawPos_ = _parent->getWallRectPos() + _parent->getCloseUIPos();
		int size_ = cCloseUIBGRadius * 2;
		rVal_.setFromCenter(drawPos_, size_, size_);
	}

	return rVal_;
}
#pragma endregion


