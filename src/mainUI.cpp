#include "mainUI.h"
#include "wallMgr.h"

#pragma region CLASS baseUnit
//--------------------------------------------------------------
mainUI::baseUnit::baseUnit(ofColor c)
	:_color(c)
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
void mainUI::baseUnit::draw(bool isBig, ofVec2f pos, int width, int height, bool displayZH)
{
	if (_eState == eClose)
	{
		return;
	}
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos);
	{
		float bpDrawWidth_ = width * _animBackplaneWidth.getCurrentValue();
		int bpDrawX_ = (bpDrawWidth_ * -0.5);
		
		ofRectangle drawRect_ = getDrawRect(isBig, width, height, displayZH);

		ofSetColor(_color, cMainUIAlpha);
		ofFill();
		ofRect(bpDrawX_, height * -0.5, bpDrawWidth_, height);
		
		ofSetColor(255);

		if (displayZH)
		{
			if (isBig)
			{
				_nameBigZH.draw(drawRect_);
			}
			else
			{
				_nameSmallZH.draw(drawRect_);
			}
		}
		else
		{
			if (isBig)
			{
				_nameBigEN.draw(drawRect_);
			}
			else
			{
				_nameSmallEN.draw(drawRect_);
			}
		}
		
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void mainUI::baseUnit::setNameBig(ofImage & zhText, ofImage & enText)
{
	_nameBigZH = zhText;
	_nameBigEN = enText;
}

//--------------------------------------------------------------
void mainUI::baseUnit::setNameSmall(ofImage & zhText, ofImage & enText)
{
	_nameSmallZH = zhText;
	_nameSmallEN = enText;
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

//--------------------------------------------------------------
ofRectangle mainUI::baseUnit::getDrawRect(bool isBig, int width, int height, bool displayZH)
{
	ofRectangle rVal_;

	float ratio_ = 0.0;
	if (displayZH)
	{
		if (isBig)
		{
			
			ratio_ = _nameBigZH.getHeight() / _nameBigZH.getWidth();
		}
		else
		{
			ratio_ = _nameSmallZH.getHeight() / _nameSmallZH.getWidth();
		}
	}
	else
	{
		if (isBig)
		{
			ratio_ = _nameBigEN.getHeight() / _nameBigEN.getWidth();
		}
		else
		{
			ratio_ = _nameSmallEN.getHeight() / _nameSmallEN.getWidth();
		}
	}

	float textDrawWidth_ = width * 0.9 * _animTextWidth.getCurrentValue();
	float textDrawHeight_ = width * 0.9 * ratio_;
	int textDrawX_ = (textDrawWidth_ * -0.5);

	rVal_.set(textDrawX_, textDrawHeight_ * -0.5, textDrawWidth_, textDrawHeight_);
	return rVal_;
}
#pragma endregion

#pragma region Basic Method
//-----------------------------------------------------------------------------
mainUI::mainUI()
	:_setup(false)
	, _parentWallMgr(nullptr)
	, _eMainUIState(eMainUIDisable)
	, _eBigUIState(eBigUIClose)
	, _eSmallUIState(eSmallUIClose)
{}

//-----------------------------------------------------------------------------
void mainUI::setup(wallMgr* wallMgr, ofVec2f drawPos, ePhotoPrimaryCategory eCategory)
{
	_category = eCategory;

	_setup = setupUI();
	setupBigMenu();
	setupSmallMenu();
	
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

	updateBigMenu(delta);
	updateSmallMenu(delta);
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

		if (_eMainUIState == eMainUIDisplayBig)
		{
			drawBigMenu(isZH);
		}
		else if(_eMainUIState == eMainUIDisplaySmall)
		{
			drawSmallMenu(isZH);
		}

	}
	ofPopMatrix();
	ofPopStyle();
	
}

//-----------------------------------------------------------------------------
void mainUI::start()
{
	_eMainUIState = eMainUIDisplayBig;
	_eBigUIState = eBigUIClose;
	_eSmallUIState = eSmallUIClose;
	disableInput();
	mainIn();

}

//-----------------------------------------------------------------------------
void mainUI::end()
{
	_eMainUIState = eMainUIDisable;
	disableInput();
	resetUI();	
}

//-----------------------------------------------------------------------------
void mainUI::toBigMenu()
{
	if (_eMainUIState != eMainUIDisable && _eSmallUIState == eSmallOpen && _eBigUIState == eBigUIClose)
	{
		smallUIOut();
		disableInput();
	}
}

//-----------------------------------------------------------------------------
void mainUI::toSmallMenu()
{
	if (_eMainUIState != eMainUIDisable && _eSmallUIState == eSmallUIClose && _eBigUIState == eBigUIOpen)
	{
		miniOut();
		disableInput();
	}
}

//-----------------------------------------------------------------------------
bool mainUI::isReady()
{
	return _eBigUIState == eBigUIOpen || _eSmallUIState == eSmallOpen;
}

//-----------------------------------------------------------------------------
void mainUI::changeCategory(ePhotoPrimaryCategory eCategory)
{
	_category = eCategory;
}

//-----------------------------------------------------------------------------
bool mainUI::setupUI()
{
	for (int idx_ = 0; idx_ < ePhotoCategory_Num; idx_++)
	{
		ePhotoPrimaryCategory type_ = (ePhotoPrimaryCategory)idx_;
		string nameTextZH_ = dataHolder::GetInstance()->getCategoryName(type_, true);
		string nameTextEN_ = dataHolder::GetInstance()->getCategoryName(type_, false);
		string subtitleTextZH_ = dataHolder::GetInstance()->getCategorySubtitle(type_, true);
		string subtitleTextEN_ = dataHolder::GetInstance()->getCategorySubtitle(type_, false);
		
		ofImage nameBigZH_, nameBigEN_;
		ofImage subtitleZH_, subtitleEN_;
		ofImage fullUIZH_, fullUIEN_;
		ofImage nameSmallZH_, nameSmallEN_;


		createTextImg(nameTextZH_, eFontType::eFontMainUI, true, nameBigZH_);
		createTextImg(nameTextEN_, eFontType::eFontMainUI, false, nameBigEN_);
		createTextImg(subtitleTextZH_, eFontType::eFontMainUISubtitle, true, subtitleZH_);
		createTextImg(subtitleTextEN_, eFontType::eFontMainUISubtitle, false, subtitleEN_);
		fullUIZH_ = combineUIName(nameBigZH_, subtitleZH_);
		fullUIEN_ = combineUIName(nameBigEN_, subtitleEN_);

		createTextImg(nameTextZH_, eFontType::eFontMainUISmall, true, nameSmallZH_);
		createTextImg(nameTextEN_, eFontType::eFontMainUISmall, false, nameSmallEN_);

		baseUnit newUnit_(getCategoryColor(type_));
		newUnit_.setNameBig(fullUIZH_, fullUIEN_);
		newUnit_.setNameSmall(nameSmallZH_, nameSmallEN_);
		_mainUIMap.insert(make_pair(type_, newUnit_));
	}
	return true;
}

//-----------------------------------------------------------------------------
void mainUI::resetUI()
{
	for (auto& iter_ : _mainUIMap)
	{
		iter_.second.directClose();
	}
	resetBigMenuAnim();
}

//-----------------------------------------------------------------------------
void mainUI::createTextImg(string text, eFontType eType, bool isZH, ofImage& img)
{
	img.clear();

	auto textRect_ = fontMgr::GetInstance()->getStringBoundingBox(eType, text, isZH);
	ofFbo	_canvas;
	_canvas.allocate(textRect_.getWidth(), textRect_.getHeight(), GL_RGBA);

	_canvas.begin();
	{
		ofClear(255);
		ofPushMatrix();

		ofSetColor(255);
		fontMgr::GetInstance()->drawString(eType, text, ofVec2f(-textRect_.x, -textRect_.y), isZH);
		ofPopMatrix();
	}
	_canvas.end();

	ofPixels pixel_;
	_canvas.readToPixels(pixel_);
	img.setFromPixels(pixel_);
}

//-----------------------------------------------------------------------------
ofImage mainUI::combineUIName(ofImage & name, ofImage & subtitle)
{
	int width_ = name.getWidth() + subtitle.getWidth();
	int height_ = name.getHeight() + subtitle.getHeight();

	ofFbo _canvas;
	_canvas.allocate(cMainUIUnitWidth, cMainUIUnitHeight, GL_RGBA);
	_canvas.begin();
	{
		ofColor(255);
		name.draw(
			(cMainUIUnitWidth - name.getWidth()) * 0.5f,
			(cMainUIUnitHeight - name.getHeight()) * 0.5 - cMainUIUnitHeight * 0.1
		);

		subtitle.draw(
			(cMainUIUnitWidth - subtitle.getWidth()) * 0.5f,
			(cMainUIUnitHeight - name.getHeight()) * 0.5 + cMainUIUnitHeight * 0.2
		);

	}
	_canvas.end();

	ofImage combineUI_;
	ofPixels pixel_;
	_canvas.readToPixels(pixel_);
	combineUI_.setFromPixels(pixel_);

	return combineUI_;
}

#pragma endregion

#pragma region Big Menu
//-----------------------------------------------------------------------------
void mainUI::setupBigMenu()
{
	_mainPos.set(cMainUIWidth * -0.5 + cMainUIUnitWidth * 0.5, cMainUIHeight * -0.5 + cMainUIUnitHeight * 0.5);
	_miniPos.set(cMainUIWidth * -0.5 + cMainUIUnitWidth + cMainUIUnitMinWidth * -0.5, cMainUIHeight * -0.5 + cMainUIUnitMinHeight * 0.5);
	_btnPos.set(cMainUIWidth * -0.5, cMainUIHeight * -0.5);

	resetBigMenuAnim();
}

//-----------------------------------------------------------------------------
void mainUI::updateBigMenu(float delta)
{
	for (auto& iter_ : _mainUIMap)
	{
		iter_.second.update(delta);
	}

	for (auto& iter_ : _animMiniPosX)
	{
		iter_.update(delta);
	}
	_animBtn.update(delta);
	bigMenuAnimStateCheck();
}

//-----------------------------------------------------------------------------
void mainUI::drawBigMenu(bool isZH)
{
	ofPushStyle();
	{
		ofSetColor(255);
		drawMini(isZH);
		drawMain(isZH);
		drawBtn(isZH);
	}		
	ofPopStyle();
}

//-----------------------------------------------------------------------------
void mainUI::drawMain(bool isZH)
{
	_mainUIMap[_category].draw(true, _mainPos, cMainUIUnitWidth, cMainUIUnitHeight, isZH);
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
			iter_.second.draw(false, categoryPos_, cMainUIUnitMinWidth, cMainUIUnitMinHeight, isZH);
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
void mainUI::resetBigMenuAnim()
{
	for (auto& iter_ : _animMiniPosX)
	{
		iter_.reset(0.0f);
		iter_.setDuration(0.2f);
		iter_.setRepeatType(AnimRepeat::PLAY_ONCE);
	}

	_animBtn.setDuration(0.4f);
	_animBtn.setRepeatType(PLAY_ONCE);
	_animBtn.reset(0.0);
}

//-----------------------------------------------------------------------------
void mainUI::mainIn()
{	
	_eMainUIState = eMainUIDisplayBig;
	_mainUIMap[_category].open();
	_eBigUIState = eBigUIMainIn;
}

//-----------------------------------------------------------------------------
void mainUI::mainOut()
{
	_mainUIMap[_category].close();
	_eBigUIState = eBigUIMainOut;
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
	_eBigUIState = eBigUIMiniIn;
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
	_eBigUIState = eBigUIMiniOut;
}

//-----------------------------------------------------------------------------
void mainUI::bigMenuInputCheck(ofVec2f pos)
{
	ofVec2f pos_ = pos - (_parentWallMgr->getWallRectPos() + _centerPos);

	if (pos_.distance(_btnPos) < (cMainUIBtnWidth * 0.5))
	{
		_parentWallMgr->changeLanguage();
	}
	else
	{
		auto selectCategory_ = getSelectMiniCategory(pos_);
		if (selectCategory_ != _category)
		{
			_parentWallMgr->changeCategory(selectCategory_);
		}
		_parentWallMgr->mainUIout();
	}
}

//-----------------------------------------------------------------------------
ofRectangle mainUI::getBigMenuInputArea()
{
	ofRectangle inputArea_;
	inputArea_.setFromCenter(_parentWallMgr->getWallRectPos() + _centerPos, cMainUIWidth, cMainUIHeight);
	inputArea_.scaleFromCenter(1.15);
	return inputArea_;
}

//-----------------------------------------------------------------------------
void mainUI::bigMenuAnimStateCheck()
{
	switch (_eBigUIState)
	{
	case eBigUIMainIn:
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
		}
		break;
	}
	case eBigUIMiniIn:
	{
		if (_animMiniPosX[2].hasFinishedAnimating() && _animMiniPosX[2].getPercentDone() == 1.0)
		{
			_eBigUIState = eBigUIOpen;
			enableInput();
		}
		break;
	}
	case eBigUIMiniOut:
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
			_eBigUIState = eBigUIMainOut;
			_mainUIMap[_category].close();
		}
		break;
	}
	case eBigUIMainOut:
	{
		if (_mainUIMap[_category].isClose())
		{
			_eBigUIState = eBigUIClose;
			smallUIIn();
		}
		break;
	}
	}
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
ePhotoPrimaryCategory mainUI::getSelectMiniCategory(ofVec2f selectPos)
{
	ePhotoPrimaryCategory rVal_ = _category;
	if (abs(_miniPos.x + cMainUIUnitMinWidth - selectPos.x) < cMainUIUnitMinWidth * 0.5)
	{
		int index_ = (int)(abs(selectPos.y - (_miniPos.y - cMainUIUnitMinHeight * 0.5)) / (float)cMainUIUnitMinHeight);

		for (auto& iter_ : _mainUIMap)
		{
			if (iter_.first != _category)
			{
				if (index_ == 0)
				{
					rVal_ = iter_.first;
					break;
				}
				index_--;
			}
		}
	}
	return rVal_;
}
#pragma endregion

#pragma region Small Menu
//-----------------------------------------------------------------------------
void mainUI::setupSmallMenu()
{
	_smallPos.set(cMainSmallUIWidth * -0.5, 0);
}

//-----------------------------------------------------------------------------
void mainUI::updateSmallMenu(float delta)
{
	smallMenuAnimStateCheck();
}

//-----------------------------------------------------------------------------
void mainUI::drawSmallMenu(bool isZH)
{
	_mainUIMap[_category].draw(false, _smallPos, cMainSmallUIWidth, cMainSmallUIHeight, isZH);
}

//-----------------------------------------------------------------------------
void mainUI::smallUIIn()
{
	_eMainUIState = eMainUIDisplaySmall;
	_mainUIMap[_category].open();
	_eSmallUIState = eSmallUIIn;
}

//-----------------------------------------------------------------------------
void mainUI::smallUIOut()
{
	_mainUIMap[_category].close();
	_eSmallUIState = eSmallUIOut;
}

//-----------------------------------------------------------------------------
void mainUI::smallMenuAnimStateCheck()
{
	switch (_eSmallUIState)
	{
		case eSmallUIIn:
		{
			if (_mainUIMap[_category].isOpen())
			{
				_eSmallUIState = eSmallOpen;
				enableInput();
			}
			break;
		}
		case eSmallUIOut:
		{
			if (_mainUIMap[_category].isClose())
			{
				_eSmallUIState = eSmallUIClose;
				mainIn();
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void mainUI::smallMenuInputCheck(ofVec2f pos)
{
	ofVec2f pos_ = pos - (_parentWallMgr->getWallRectPos() + _centerPos);

	if (abs(pos_.x - _smallPos.x) < cMainSmallUIWidth * 0.5 &&
		abs(pos_.y - _smallPos.y) < cMainSmallUIHeight * 0.5)
	{
		_parentWallMgr->mainUIin();
	}
}

//-----------------------------------------------------------------------------
ofRectangle mainUI::getSmallMenuInputArea()
{
	ofRectangle inputArea_;
	inputArea_.setFromCenter(_parentWallMgr->getWallRectPos() + _centerPos + _smallPos, cMainSmallUIWidth, cMainSmallUIHeight);
	return inputArea_;
}
#pragma endregion

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
	if (_eMainUIState == eMainUIDisplayBig)
	{
		bigMenuInputCheck(e.pos);
	}
	else if(_eMainUIState == eMainUIDisplaySmall)
	{
		smallMenuInputCheck(e.pos);
	}	
}

//--------------------------------------
ofRectangle mainUI::getInputArea()
{
	if (_eMainUIState == eMainUIDisplayBig)
	{
		return getBigMenuInputArea();
	}
	else if (_eMainUIState == eMainUIDisplaySmall)
	{
		return getSmallMenuInputArea();
	}
	else
	{
		return ofRectangle();
	}
}
#pragma endregion
