#include "textUI.h"
#include "wallMgr.h"
//---------------------------------
textUI::textUI()
	:_isSetup(false)
	, _eState(eClose)
{}

//---------------------------------
void textUI::setup(wallMgr * parent)
{
	_parent = parent;

	_displayCanvas.allocate(cTextUIWidth, cTextUIHeight);

	_animHeight.setDuration(cTextUIAnimDuration);
	_animHeight.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animHeight.reset(0.0f);
	_eState = eClose;
	_isSetup = true;
}

//---------------------------------
void textUI::update(float delta)
{
	if (!_isSetup)
	{
		return;
	}

	_animHeight.update(delta);
	animStateCheck();
}

//---------------------------------
void textUI::draw(ofVec2f pos)
{
	if (!_isSetup)
	{
		return;
	}

	ofPushMatrix();
	ofTranslate(pos);
	ofPushStyle();
	ofSetColor(255);
	{
		_displayCanvas.draw(cTextUIWidth * -0.5f, 0, cTextUIWidth, _animHeight.getCurrentValue() * cTextUIHeight);
	}
	ofPopMatrix();
	ofPopStyle();
}

//---------------------------------
void textUI::updateText(string & title, string & context, bool isZH)
{
	auto titleFontType_ = isZH ? eFontTextUI : eFontTextUI;
	auto titleBoundingBox_ = fontMgr::GetInstance()->getStringBoundingBox(titleFontType_, title, isZH);
	
	auto contextFontType_ = isZH ? eFontTextUIContext : eFontTextUIContext;
	auto eachWordWidth_ = fontMgr::GetInstance()->getEachWordWidth(contextFontType_, context, isZH);
	string processContext_ = reconstructMsg(cTextUIContextLimitWidth, context, eachWordWidth_);
	auto contextBoundingBox_ = fontMgr::GetInstance()->getStringBoundingBox(contextFontType_, processContext_, isZH);

	_textCanvas.clear();
	_textCanvas.allocate(
		cTextUIWidth
		, titleBoundingBox_.getHeight() * 1.2 + contextBoundingBox_.getHeight()
	);

	_textCanvas.begin();
	ofSetColor(255);
	fontMgr::GetInstance()->drawString(titleFontType_, title, -titleBoundingBox_.getPosition(), isZH);
	fontMgr::GetInstance()->drawString(
		contextFontType_,
		processContext_,
		ofVec2f(-contextBoundingBox_.getX(), titleBoundingBox_.getHeight() * 0.2 -(titleBoundingBox_.getY() + contextBoundingBox_.getY())),
		isZH
	);
	_textCanvas.end();

	_textDrawY = 0;
	_textDrawYMin = MIN(0, -(_textCanvas.getHeight() - cTextUIHeight));
	updateCanvas();
}

//---------------------------------
void textUI::open()
{
	if (!_isSetup || _eState != eClose)
	{
		return;
	}

	_animHeight.animateTo(1.0f);
	_eState = eUIIn;
}

//---------------------------------
void textUI::close()
{
	if (!_isSetup || _eState != eOpen)
	{
		return;
	}

	_animHeight.animateTo(0.0f);
	_eState = eUIOut;
}

//---------------------------------
void textUI::updateCanvas()
{
	_displayCanvas.begin();
	{
		ofClear(0.0f, 128.0f);
		_textCanvas.draw(0, _textDrawY);
	}
	_displayCanvas.end();
}

//---------------------------------
void textUI::animStateCheck()
{
	switch (_eState)
	{
	case eUIIn:
	{
		if (_animHeight.hasFinishedAnimating() && _animHeight.getPercentDone() == 1.0f)
		{
			_eState = eOpen;
		}
		break;
	}
	case eUIOut:
	{
		if (_animHeight.hasFinishedAnimating() && _animHeight.getPercentDone() == 1.0f)
		{
			_eState = eClose;
		}
		break;
	}
	}
}

//---------------------------------
bool textUI::moveTextDrawY(int delta)
{
	int afterMove_ = _textDrawY + delta;
	afterMove_ = MAX(MIN(afterMove_, 0), _textDrawYMin);
	
	if (_textDrawY == afterMove_)
	{
		return false;
	}
	else
	{
		_textDrawY = afterMove_;
		return true;
	}
}

//---------------------------------
string textUI::reconstructMsg(int widthLimit, string & msg, vector<int>& eachWordWidth)
{
	wstring wMsg_ = fontMgr::s2ws(msg);
	int tempWidth_ = 0;
	int insertCounter_ = 0;
	for(int idx_ = 0; idx_ < eachWordWidth.size(); idx_++)
	{
		if (tempWidth_ + eachWordWidth[idx_] > widthLimit)
		{
			wMsg_.insert(idx_ + insertCounter_, L"\n");
			tempWidth_ = 0;
			insertCounter_++;
		}

		tempWidth_ += eachWordWidth[idx_];
	}
	return fontMgr::ws2s(wMsg_);
}

#pragma region Input
//---------------------------------
void textUI::enableInput(bool isSelect)
{
	inputEventMgr::GetInstance()->registerInputEvent(this, eInputSystemLevel::eInputTextUI);
}

//---------------------------------
void textUI::disableInput()
{
	inputEventMgr::GetInstance()->unregisterInputEvent(this);
}

//---------------------------------
void textUI::inputDrag(inputEventArgs e)
{	
	if (moveTextDrawY(e.delta.y))
	{
		updateCanvas();
	}
}

//---------------------------------
ofRectangle textUI::getInputArea()
{
	ofRectangle rVal_;
	if (_isSetup && _eState == eOpen)
	{
		auto drawPos_ = _parent->getWallRectPos() + _parent->getTextUIPos();
		drawPos_.x -= cTextUIWidth * 0.5;
		rVal_.set(drawPos_, cTextUIWidth, cTextUIHeight);
	}
	
	return rVal_;
}
#pragma endregion


