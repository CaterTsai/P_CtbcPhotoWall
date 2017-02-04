#include "mainUI.h"
#include "wallMgr.h"
#pragma region CLASS baseUnit
//--------------------------------------------------------------
mainUI::baseUnit::baseUnit(ofImage& zhText, ofRectangle& zhRect, ofImage& enText, ofRectangle& enRect, ofColor c)
	:_color(c)
	, _textZH(zhText)
	, _textRectZH(zhRect)
	,_textEN(enText)
	, _textRectEN(enRect)
	, _eState(eClose)
{
	_animTextWidth.setDuration(0.2);
	_animTextWidth.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animTextWidth.reset(0.0);

	_animBackplaneWidth.setDuration(0.2);
	_animBackplaneWidth.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animBackplaneWidth.reset(0.0);
}

//--------------------------------------------------------------
void mainUI::baseUnit::update(float delta)
{
	_animTextWidth.update(delta);
	_animBackplaneWidth.update(delta);

	switch (_eState)
	{
		case eBPIn:
		{
			if (_animBackplaneWidth.hasFinishedAnimating() && _animBackplaneWidth.getPercentDone() == 1.0)
			{
				_eState = eTextIn;
				_animTextWidth.animateFromTo(0.0, 1.0);
			}
			break;
		}
		case eTextIn:
		{
			if (_animTextWidth.hasFinishedAnimating() && _animTextWidth.getPercentDone() == 1.0)
			{
				_eState = eOpen;
			}
			break;
		}
		case eTextOut:
		{
			if (_animTextWidth.hasFinishedAnimating() && _animTextWidth.getPercentDone() == 1.0)
			{
				_eState = eBPOut;
				_animBackplaneWidth.animateFromTo(1.0, 0.0);
			}
			break;
		}
		case eBPOut:
		{
			if (_animBackplaneWidth.hasFinishedAnimating() && _animBackplaneWidth.getPercentDone() == 1.0)
			{
				_eState = eClose;
			}
			break;
		}
	}
}

//--------------------------------------------------------------
void mainUI::baseUnit::draw(ofVec2f pos, int width, int height, bool displayZH)
{
	if (_eState == eClose)
	{
		return;
	}
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos);
	{
		float ratio_ = displayZH ? (_textRectZH.getHeight() / _textRectZH.getWidth()) :(_textRectEN.getHeight() / _textRectEN.getWidth());
		float textHeight_ = width * 0.7 * ratio_;
		float textDrawWidth_ = width * 0.7* _animTextWidth.getCurrentValue();
		float textDrawHeight_ = textHeight_;
		float bpDrawWidth_ = width * _animBackplaneWidth.getCurrentValue();
		int textDrawX_ = (textDrawWidth_ * -0.5);
		int bpDrawX_ = (bpDrawWidth_ * -0.5);
		

		ofSetColor(_color, cMainUIAlpha);
		ofFill();
		ofRect(bpDrawX_, height * -0.5, bpDrawWidth_, height);

		ofSetColor(255);

		if (displayZH)
		{
			_textZH.draw(textDrawX_, textDrawHeight_ * -0.5, textDrawWidth_, textDrawHeight_);
		}
		else
		{
			_textEN.draw(textDrawX_, textDrawHeight_ * -0.5, textDrawWidth_, textDrawHeight_);
		}
		
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void mainUI::baseUnit::open()
{
	if (_eState != eClose)
	{
		return;
	}

	_animBackplaneWidth.animateFromTo(0.0, 1.0);
	_eState = eBPIn;
}

//--------------------------------------------------------------
void mainUI::baseUnit::close()
{
	if (_eState != eOpen)
	{
		return;
	}

	_animTextWidth.animateFromTo(1.0, 0.0);
	_eState = eTextOut;
}

//--------------------------------------------------------------
bool mainUI::baseUnit::isOpen()
{
	return _eState == eOpen;
}

//--------------------------------------------------------------
bool mainUI::baseUnit::isClose()
{
	return _eState == eClose;
}

//--------------------------------------------------------------
void mainUI::baseUnit::directOpen()
{
	_eState = eOpen;
	_animTextWidth.reset(1.0);
	_animBackplaneWidth.reset(1.0);
}

//--------------------------------------------------------------
void mainUI::baseUnit::directClose()
{
	_eState = eClose;
	_animTextWidth.reset(0.0);
	_animBackplaneWidth.reset(0.0);
}
#pragma endregion

//-----------------------------------------------------------------------------
mainUI::mainUI()
	:_setup(false)
	, _parentWallMgr(nullptr)
{}

//-----------------------------------------------------------------------------
void mainUI::setup(wallMgr* wallMgr, ofVec2f drawPos, ePhotoPrimaryCategory eCategory)
{

	_category = eCategory;

	setupMiniAnim();
	_animBtn.setDuration(0.4f);
	_animBtn.setRepeatType(PLAY_ONCE);
	_animBtn.reset(0.0);

	_mainPos.set(cMainUIWidth * -0.5 + cMainUIUnitWidth * 0.5, cMainUIHeight * -0.5 + cMainUIUnitHeight * 0.5);
	_miniPos.set(cMainUIWidth * -0.5 + cMainUIUnitWidth + cMainUIUnitMinWidth * -0.5, cMainUIHeight * -0.5 + cMainUIUnitMinHeight * 0.5);
	_btnPos.set(cMainUIWidth * -0.5, cMainUIHeight * -0.5);
	_setup = setupUI();

	_parentWallMgr = wallMgr;
	_centerPos = drawPos;

}

//-----------------------------------------------------------------------------
void mainUI::update(float delta)
{
	if (!_setup)
	{
		return;
	}

	for (auto& iter_ : _mainUIMap)
	{
		iter_.second.update(delta);
	}

	for (auto& iter_ : _animMiniPosX)
	{
		iter_.update(delta);
	}
	_animBtn.update(delta);
	animStateCheck();
}

//-----------------------------------------------------------------------------
void mainUI::draw(bool isZH)
{
	if (!_setup)
	{
		return;
	}

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(_centerPos);
	{
		ofSetColor(255);
		drawMini(isZH);
		drawMain(isZH);
		drawBtn(isZH);

	}
	ofPopMatrix();
	ofPopStyle();
}

//-----------------------------------------------------------------------------
void mainUI::open()
{
	if (_eUIState != eUIClose)
	{
		return;
	}

	_eUIState = eUIMainIn;
	_mainUIMap[_category].open();
	
}

//-----------------------------------------------------------------------------
void mainUI::close()
{
	if (_eUIState != eUIOpen)
	{
		return;
	}

	disableInput();
	_eUIState = eUIMiniOut;
	miniOut();
}

//-----------------------------------------------------------------------------
void mainUI::drawMain(bool isZH)
{	
	_mainUIMap[_category].draw(_mainPos, cMainUIUnitWidth, cMainUIUnitHeight, isZH);
}

//-----------------------------------------------------------------------------
void mainUI::drawMini(bool isZH)
{	
	int idx_ = 0;
	for (auto& iter_ : _mainUIMap)
	{
		ofVec2f categoryPos_ = _miniPos;
		categoryPos_.y += cMainUIUnitMinHeight * idx_;
		if (iter_.first != _category)
		{
			categoryPos_.x += _animMiniPosX[idx_].getCurrentValue() * cMainUIUnitMinWidth;
			iter_.second.draw(categoryPos_, cMainUIUnitMinWidth, cMainUIUnitMinHeight, isZH);
			idx_++;
		}
	}
}

//-----------------------------------------------------------------------------
void mainUI::drawBtn(bool isZH)
{
	float _animVal = _animBtn.getCurrentValue();
	ofPushMatrix();
	ofTranslate(_btnPos);
	ofRotateZ(_animVal * 360.0);
	{
		int drawWidth_ = cMainUIBtnWidth * _animVal;
		int drawHeight_ = cMainUIBtnHeight * _animVal;
		imageRender::GetInstance()->drawImage(
			getBtnName(_category, isZH),
			ofVec2f(drawWidth_ * -0.5f, drawHeight_ * -0.5f),
			drawWidth_,
			drawHeight_
		);
	}
	ofPopMatrix();
}

//-----------------------------------------------------------------------------
void mainUI::setupMiniAnim()
{
	for (auto& iter_ : _animMiniPosX)
	{
		iter_.reset(0.0f);
		iter_.setDuration(0.2f);
		iter_.setRepeatType(AnimRepeat::PLAY_ONCE);
	}
}

//-----------------------------------------------------------------------------
void mainUI::miniIn()
{
	for (auto& iter_ : _animMiniPosX)
	{
		iter_.reset(0.0f);
	}
	_animMiniPosX[0].animateTo(1.0f);
	_animMiniPosX[1].animateToAfterDelay(1.0f, 0.1f);
	_animMiniPosX[2].animateToAfterDelay(1.0f, 0.2f);

	_animBtn.animateTo(1.0f);
}

//-----------------------------------------------------------------------------
void mainUI::miniOut()
{
	for (auto& iter_ : _animMiniPosX)
	{
		iter_.reset(1.0f);
	}
	_animMiniPosX[0].animateTo(0.0f);
	_animMiniPosX[1].animateToAfterDelay(0.0f, 0.1f);
	_animMiniPosX[2].animateToAfterDelay(0.0f, 0.2f);

	_animBtn.animateTo(0.0f);
}

//-----------------------------------------------------------------------------
void mainUI::animStateCheck()
{
	switch (_eUIState)
	{
		case eUIMainIn:
		{
			if (_mainUIMap[_category].isOpen())
			{
				for (auto& iter_ : _mainUIMap)
				{
					if (iter_.first != _category)
					{
						iter_.second.directOpen();
					}
				}
				miniIn();
				_eUIState = eUIMiniIn;
			}
			break;
		}
		case eUIMiniIn:
		{
			if (_animMiniPosX[2].hasFinishedAnimating() && _animMiniPosX[2].getPercentDone() == 1.0)
			{
				_eUIState = eUIOpen;
				enableInput();
			}
			break;
		}
		case eUIMiniOut:
		{
			if (_animMiniPosX[2].hasFinishedAnimating() && _animMiniPosX[2].getPercentDone() == 1.0)
			{
				for (auto& iter_ : _mainUIMap)
				{
					if (iter_.first != _category)
					{
						iter_.second.directClose();
					}
				}
				_eUIState = eUIMainOut;
				_mainUIMap[_category].close();
			}
			break;
		}
		case eUIMainOut:
		{
			if (_mainUIMap[_category].isClose())
			{
				_eUIState = eUIClose;
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
bool mainUI::setupUI()
{
	for (int idx_ = 0; idx_ < ePhotoCategory_Num; idx_++)
	{
		ePhotoPrimaryCategory type_ = (ePhotoPrimaryCategory)idx_;
		string zh_ = dataHolder::GetInstance()->getCategoryName(type_, true);
		string en_ = dataHolder::GetInstance()->getCategoryName(type_, false);

		ofImage enImage_, zhImage_;
		ofRectangle enRect_, zhRect_;
		createTextImgEN(en_, enImage_, enRect_);
		createTextImg(zh_, zhImage_, zhRect_);

		baseUnit newUnit_(zhImage_, zhRect_, enImage_, enRect_, getCategoryColor(type_));
		_mainUIMap.insert(make_pair(type_, newUnit_));

	}
	return true;
}

//-----------------------------------------------------------------------------
string mainUI::getBtnName(ePhotoPrimaryCategory category, bool isZH)
{
	string btnName_ = "";
	switch (category)
	{
	case ePhotoCategory_1:
	{
		btnName_ = isZH ? NAME_MGR::I_BtnEN1 : NAME_MGR::I_BtnZH1;
		break;
	}
	case ePhotoCategory_2:
	{
		btnName_ = isZH ? NAME_MGR::I_BtnEN2 : NAME_MGR::I_BtnZH2;
		break;
	}
	case ePhotoCategory_3:
	{
		btnName_ = isZH ? NAME_MGR::I_BtnEN3 : NAME_MGR::I_BtnZH3;
		break;
	}
	case ePhotoCategory_4:
	{
		btnName_ = isZH ? NAME_MGR::I_BtnEN4 : NAME_MGR::I_BtnZH4;
		break;
	}
	default:
	{
		btnName_ = "";
	}
	}
	return btnName_;
}

//-----------------------------------------------------------------------------
void mainUI::createTextImg(string text, ofImage& img, ofRectangle& textRect)
{
	img.clear();
	
	textRect = fontMgr::GetInstance()->getStringBoundingBox(eFontType::eFontMainUIZH, text);
	ofFbo	_canvas;
	_canvas.allocate(textRect.getWidth(), textRect.getHeight(), GL_RGBA);

	_canvas.begin();
	{
		ofClear(0);
		ofPushMatrix();

		ofSetColor(255);
		fontMgr::GetInstance()->drawString(eFontType::eFontMainUIZH, text, ofVec2f(-textRect.x, -textRect.y));
		ofPopMatrix();
	}
	_canvas.end();

	ofPixels pixel_;
	_canvas.readToPixels(pixel_);
	img.setFromPixels(pixel_);
}

//-----------------------------------------------------------------------------
void mainUI::createTextImgEN(string text, ofImage& img, ofRectangle& textRect)
{
	img.clear();

	textRect = fontMgr::GetInstance()->getStringBoundingBox(eFontType::eFontMainUIEN, text);
		
	ofFbo	_canvas;
	_canvas.allocate(textRect.getWidth(), textRect.getHeight(), GL_RGBA);

	_canvas.begin();
	{
		ofClear(0);
		ofPushMatrix();
		
		ofSetColor(255);
		fontMgr::GetInstance()->drawString(eFontType::eFontMainUIEN, text, ofVec2f(-textRect.x, -textRect.y));
		ofPopMatrix();
	}
	_canvas.end();

	ofPixels pixel_;
	_canvas.readToPixels(pixel_);
	img.setFromPixels(pixel_);
}

#pragma region Input
//--------------------------------------
void mainUI::enableInput()
{
	inputEventMgr::GetInstance()->registerInputEvent(this, eInputMainUI);
}

//--------------------------------------
void mainUI::disableInput()
{
	inputEventMgr::GetInstance()->unregisterInputEvent(this);
}

//--------------------------------------
void mainUI::inputRelease(inputEventArgs e)
{
	ofVec2f pos_ = e.pos - (_parentWallMgr->getWallRectPos() + _centerPos);
	
	if (pos_.distance(_btnPos) < (cMainUIBtnWidth * 0.5))
	{
		_parentWallMgr->changeLanguage();
	}
	else
	{
		_parentWallMgr->mainUIout();
	}
	
}

//--------------------------------------
ofRectangle mainUI::getInputArea()
{
	ofRectangle inputArea_;
	inputArea_.setFromCenter(_parentWallMgr->getWallRectPos() + _centerPos, cMainUIWidth, cMainUIHeight);
	inputArea_.scaleFromCenter(1.15);
	return inputArea_;
}
#pragma endregion