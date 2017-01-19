#include "mainUI.h"
#include "wallMgr.h"
#pragma region CLASS baseUnit
//--------------------------------------------------------------
mainUI::baseUnit::baseUnit(ofImage& zhText, ofImage& enText, ofColor c)
	:_color(c)
	,_textZH(zhText)
	,_textEN(enText)
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
		float textWidth_ = height * (_textZH.getWidth() / _textZH.getHeight());
		float textDrawWidth_ = textWidth_ * _animTextWidth.getCurrentValue();
		float textDrawHeight_ = height;
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
void mainUI::setup(wallMgr* wallMgr, string xmlPath, ePhotoPrimaryCategory eCategory)
{
	_font.setGlobalDpi(72);
	_font.setLetterSpacing(1.2);
	_font.loadFont("fonts/black.otf", cMainUIFontSize);
	_category = eCategory;

	setupMiniAnim();
	_mainPos.set(cMainUIWidth * -0.5 + cMainUIUnitWidth * 0.5, cMainUIHeight * -0.5 + cMainUIUnitHeight * 0.5);
	_miniPos.set(cMainUIWidth * -0.5 + cMainUIUnitWidth + cMainUIUnitMinWidth * -0.5, cMainUIHeight * -0.5 + cMainUIUnitMinHeight * 0.5);

	_setup = loadXml(xmlPath);

	_parentWallMgr = wallMgr;
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
	animStateCheck();
}

//-----------------------------------------------------------------------------
void mainUI::draw(ofVec2f pos)
{
	if (!_setup)
	{
		return;
	}

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos);
	{
		ofSetColor(255);
		drawMini();
		drawMain();

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

	_eUIState = eUIMiniOut;
	miniOut();
}

//-----------------------------------------------------------------------------
void mainUI::drawMain()
{	
	_mainUIMap[_category].draw(_mainPos, cMainUIUnitWidth, cMainUIUnitHeight);
}

//-----------------------------------------------------------------------------
void mainUI::drawMini()
{	
	int idx_ = 0;
	for (auto& iter_ : _mainUIMap)
	{
		ofVec2f categoryPos_ = _miniPos;
		categoryPos_.y += cMainUIUnitMinHeight * idx_;
		if (iter_.first != _category)
		{
			categoryPos_.x += _animMiniPosX[idx_].getCurrentValue() * cMainUIUnitMinWidth;
			iter_.second.draw(categoryPos_, cMainUIUnitMinWidth, cMainUIUnitMinHeight);
			idx_++;
		}
	}
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
bool mainUI::loadXml(string xmlPath)
{
	ofxXmlSettings xml_;
	if (!xml_.load(xmlPath))
	{
		ofLog(OF_LOG_ERROR, "[mainUI::loadXml]load config failed");
		return false;
	}

	int num_ = xml_.getNumTags("mainUIText");
	for (int idx_ = 0; idx_ < num_; idx_++)
	{
		ePhotoPrimaryCategory type_ = (ePhotoPrimaryCategory)xml_.getValue("mainUIText:type", idx_, idx_);
		string zh_ = xml_.getValue("mainUIText:zh", "", idx_);
		string en_ = xml_.getValue("mainUIText:en", "", idx_);

		ofImage enImage_, zhImage_;
		createTextImg(en_, enImage_);
		createTextImg(zh_, zhImage_);

		baseUnit newUnit_(zhImage_, enImage_, getBPColor(type_));
		_mainUIMap.insert(make_pair(type_, newUnit_));

	}
	return true;
}

//-----------------------------------------------------------------------------
void mainUI::createTextImg(string text, ofImage& img)
{
	img.clear();
	
	auto msgRect_ = _font.getStringBoundingBox(text, 0, 0);

	ofFbo	_canvas;
	_canvas.allocate(cMainUIUnitWidth, cMainUIUnitHeight, GL_RGBA);
	
	_canvas.begin();
	{
		ofClear(0);
		ofPushMatrix();
		ofTranslate(cMainUIUnitWidth * 0.5, cMainUIUnitHeight * 0.5);
		ofSetColor(255);
		_font.drawString(text, msgRect_.getWidth() * -0.5f, msgRect_.getHeight() * 0.5f);
		ofPopMatrix();
	}
	_canvas.end();

	ofPixels pixel_;
	_canvas.readToPixels(pixel_);	
	img.setFromPixels(pixel_);
}

//-----------------------------------------------------------------------------
ofColor mainUI::getBPColor(ePhotoPrimaryCategory category)
{
	ofColor returnColor_;
	switch (category)
	{
		case ePhotoCategory_1:
		{
			returnColor_.set(0, 167, 157);
			break;
		}
		case ePhotoCategory_2:
		{
			returnColor_.set(254, 188, 16);
			break;
		}
		case ePhotoCategory_3:
		{
			returnColor_.set(245, 130, 59);
			break;
		}
		case ePhotoCategory_4:
		{
			returnColor_.set(240, 78, 104);
			break;
		}
		default:
		{
			returnColor_.set(0);
		}
	}
	return returnColor_;
}

#pragma region Input
//--------------------------------------
void mainUI::setupInput()
{
	inputEventMgr::GetInstance()->registerInputEvent(this, eInputMainUI);
}

//--------------------------------------
void mainUI::inputRelease(inputEventArgs e)
{
	_parentWallMgr->mainUIout();
}
	

//--------------------------------------
ofRectangle mainUI::getInputArea()
{
	return ofRectangle();
}
#pragma endregion