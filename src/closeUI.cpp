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
		ofSetColor(cCloseUIBGColor, 128);
		ofRect(cCloseUIBGSize * -0.5, cCloseUIBGSize * -0.5, cCloseUIBGSize, cCloseUIBGSize);

		ofSetColor(255);		
		imageRender::GetInstance()->drawImage(
			NAME_MGR::I_BtnBack,
			ofRectangle(cCloseUISize * -0.5, cCloseUISize * -0.5, cCloseUISize, cCloseUISize)
		);

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
		int size_ = cCloseUIBGSize * 2;
		rVal_.setFromCenter(drawPos_, size_, size_);
	}

	return rVal_;
}
#pragma endregion


