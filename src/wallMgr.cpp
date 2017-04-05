#include "wallMgr.h"


//--------------------------------
wallMgr::wallMgr()
	:_isSetup(false)
	, _selectWallList(nullptr)
	, _eWallState(eWallIdle)
	, _isScrollUIVisible(false)
	, _isTextUIVisible(false)
	, _needUpdateText(false)
	, _isDisplayZH(true)
	, _canSelect(true)
{}

//--------------------------------
wallMgr::~wallMgr()
{
	_wallListMgr.clear();
}

#pragma region Base Method
//--------------------------------
void wallMgr::setup(ePhotoPrimaryCategory category, ofRectangle wallRect)
{
	_eCategory = _backupCategory = category;
	_wallRect = wallRect;
	
	_mainUI.setup(this, ofVec2f(cPhotoWallCategoryWidth * 0.5, cWindowHeight * 0.5), category);
	_textUI.setup(this);
	_scrollUI.setup(this);
	_closeUI.setup(this);
	
	ofAddListener(dataHolder::GetInstance()->_onPhotoDataLoad, this, &wallMgr::onTextIsLoad);

	_isSetup = true;
}

//--------------------------------
void wallMgr::update(float delta)
{
	setupCheck();

	for (auto& iter_ : _wallListMgr)
	{
		iter_->update(delta);
	}
	_mainUI.update(delta);

	checkUpdateText();
	_textUI.update(delta);
	_scrollUI.update(delta);
}

//--------------------------------
void wallMgr::draw()
{
	setupCheck();

	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(_wallRect.getPosition());
	ofVec2f drawPos_(0);
	for (auto& iter_ : _wallListMgr)
	{
		if (iter_->getIsDeselect())
		{
			iter_->draw();
		}
	}

	_mainUI.draw(_isDisplayZH);
	

	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------
void wallMgr::drawSelect()
{
	setupCheck();

	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(_wallRect.getPosition());
	drawScrollUI();
	for (auto& iter_ : _wallListMgr)
	{
		if (!iter_->getIsDeselect())
		{
			iter_->draw();
		}
	}
	drawTextUI();
	drawCloseUI();
	
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------
void wallMgr::drawShadow()
{
	setupCheck();
	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(_wallRect.getPosition());
	ofVec2f drawPos_(0);
	for (auto& iter_ : _wallListMgr)
	{
		if (iter_->getIsSelect())
		{
			//Shadow
			imageRender::GetInstance()->drawImage(
				NAME_MGR::I_Gradient,
				ofVec2f(iter_->getBasePosX() - cSelectShdowWidth * 0.5f, 0),
				cSelectShdowWidth,
				cWindowHeight
			);
		}
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------
void wallMgr::start()
{	
	_mainUI.start();
	_eWallState = eWallMainUI;
	enableInput();
}

//--------------------------------
void wallMgr::end()
{
	if (_eCategory != _backupCategory)
	{
		changeCategory(_backupCategory);
	}
	else
	{
		resetWallList();
	}
	
	if (_eWallState == eWallPhoto && _selectWallList != nullptr)
	{
		selectCheck(_selectWallList);
	}

	_isDisplayZH = true;
	_mainUI.end();	

	disableWallListInput();
	disableInput();
	_eWallState = eWallIdle;
}

//--------------------------------
void wallMgr::addWallList(int width, bool canBeSmile)
{
	setupCheck();
	ofRectangle	rect_(getListTotalWidth(), 0, width, _wallRect.getHeight());
	_wallListMgr.push_back(ofPtr<wallList>(new wallList(this, _eCategory, rect_, canBeSmile)));
}

//--------------------------------
ofVec2f wallMgr::getWallRectPos()
{
	return _wallRect.getPosition();
}

//--------------------------------
int wallMgr::getWallRectWidth()
{
	return _wallRect.getWidth();
}

//--------------------------------
void wallMgr::setupCheck()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("setupCheck : need setup"));
	}
}

//--------------------------------
int wallMgr::getListTotalWidth()
{
	int totalWidth_ = 0;
	for (auto& iter_ : _wallListMgr)
	{
		totalWidth_ += iter_->getBaseWidth();
	}

	return totalWidth_;
}

//--------------------------------
void wallMgr::resetWallList()
{
	for (auto& iter_ : _wallListMgr)
	{
		iter_->reset();
	}
}

//--------------------------------
void wallMgr::enableWallListInput()
{
	for (auto& iter_ : _wallListMgr)
	{
		iter_->enableInput();
	}
}

//--------------------------------
void wallMgr::disableWallListInput()
{
	for (auto& iter_ : _wallListMgr)
	{
		iter_->disableInput();
	}
}
#pragma endregion

#pragma region mainUI
//--------------------------------
void wallMgr::mainUIin()
{
	_mainUI.toBigMenu();
	_eWallState = eWallMainUI;
	enableInput();

	disableWallListInput();
	_canSelect = false;
}

//--------------------------------
void wallMgr::mainUIout()
{
	_mainUI.toSmallMenu();
	_eWallState = eWallPhoto;
	disableInput();

	enableWallListInput();
	updateScrollUI();

	_canSelect = true;
}

//--------------------------------
void wallMgr::changeLanguage()
{
	_isDisplayZH ^= true;
}

//--------------------------------
void wallMgr::changeCategory(ePhotoPrimaryCategory eCategory)
{
	for (auto& iter_ : _wallListMgr)
	{
		iter_->changeCategory(eCategory);
	}
	_mainUI.changeCategory(eCategory);
	_eCategory = eCategory;
}

#pragma endregion

#pragma region textUI
//--------------------------------
void wallMgr::textUIin()
{
	_isTextUIVisible = true;
	_textUI.open();
	_textUI.enableInput();
}

//--------------------------------
void wallMgr::textUIout()
{
	_isTextUIVisible = false;
	_textUI.close();
	_textUI.disableInput();
}

//--------------------------------
void wallMgr::setTextUIVisible(bool val)
{
	_isTextUIVisible = val;

	val ? _textUI.enableInput() : _textUI.disableInput();
}

//--------------------------------
void wallMgr::updateTextUI(int photoID)
{
	string title_, msg_;
	title_ = msg_ = "Loading";
	dataHolder::GetInstance()->getPhotoText(photoID, _isDisplayZH, title_, msg_);
	
	//TODO
	if (_isDisplayZH)
	{
		_textUI.updateText(title_, msg_, true);
	}
	else
	{
		title_ = " ";
		msg_ = " ";
		//_textUI.updateText(title_, msg_, false);
	}
	
	
}

//--------------------------------
ofVec2f wallMgr::getTextUIPos()
{
	ofVec2f rVal_ = ofVec2f(0);
	if (_selectWallList)
	{
		rVal_.set(_selectWallList->getDrawPosX(), _selectWallList->getSelectBottomPosY());
	}
	return rVal_;
}

//--------------------------------
void wallMgr::checkUpdateText()
{
	if (_needUpdateText)
	{
		updateTextUI(_selectWallList->getSelectPhotoID());
		_needUpdateText = false;
	}
}

//--------------------------------
void wallMgr::onTextIsLoad(int & pid)
{
	if (_selectWallList && pid == _selectWallList->getSelectPhotoID())
	{
		_needUpdateText = true;
	}
}

//--------------------------------
void wallMgr::drawTextUI()
{
	if (_isTextUIVisible && _selectWallList)
	{
		_textUI.draw(getTextUIPos());
	}
}

#pragma endregion

#pragma region scrollUI
//--------------------------------
void wallMgr::scrollUIin()
{
	_isScrollUIVisible = true;
	PHOTO_TYPE selectType_ = _selectWallList->getSelectType();

	if (_wallRect.getWidth() < _selectWallList->getDrawPosX() + cSelectWidth * 0.5 + cScrollUIWidth)
	{
		_scrollUI.open(false, selectType_);
	}
	else
	{
		_scrollUI.open(true, selectType_);
	}
	
	_scrollUI.enableInput();
}

//--------------------------------
void wallMgr::scrollUIout()
{
	_isScrollUIVisible = false;
	_scrollUI.close();
	_scrollUI.disableInput();
}

//--------------------------------
void wallMgr::setScrollUIVisible(bool val)
{
	_isScrollUIVisible = val;
	val ? _scrollUI.enableInput() : _scrollUI.disableInput();
}

//--------------------------------
void wallMgr::updateScrollUI()
{
	_scrollUI.updateScroll(_eCategory, _isDisplayZH);
}

//--------------------------------
ofVec2f wallMgr::getScrollUIPos()
{
	ofVec2f rVal_ = ofVec2f(0);
	if (_selectWallList)
	{
		rVal_.set(
			_selectWallList->getDrawPosX(),
			_selectWallList->getSelectTopPosY()
		);
	}
	return rVal_;
}

//--------------------------------
void wallMgr::selectType(PHOTO_TYPE type)
{
	if (_selectWallList && _selectWallList->getCanSelectType())
	{
		setTextUIVisible(false);
		setScrollUIVisible(false);
		setCloseUIVisible(false);

		_selectWallList->selectType(type);
	}
}

//--------------------------------
void wallMgr::drawScrollUI()
{
	if (_isScrollUIVisible && _selectWallList)
	{	
		_scrollUI.draw(getScrollUIPos());
	}
}

#pragma endregion

#pragma region closeUI
//--------------------------------
void wallMgr::closeUIin()
{
	_closeUI.setDisplay(true);
	_closeUI.enableInput();
	_isCloseUIVidible = true;
}

//--------------------------------
void wallMgr::closeUIout()
{
	_closeUI.setDisplay(false);
	_closeUI.disableInput();
	_isCloseUIVidible = false;
}

//--------------------------------
void wallMgr::setCloseUIVisible(bool val)
{
	_isCloseUIVidible = val;
	val ? _closeUI.enableInput() : _closeUI.disableInput();
}

//--------------------------------
void wallMgr::closeSelect()
{
	if (_selectWallList)
	{
		selectCheck(_selectWallList);
	}
}

//--------------------------------
ofVec2f wallMgr::getCloseUIPos()
{
	ofVec2f rVal_ = ofVec2f(0);
	if (_selectWallList)
	{
		rVal_.set(
			_selectWallList->getDrawPosX() + cSelectWidth * -0.5 + cCloseUIBGSize,
			_selectWallList->getSelectBottomPosY() + cCloseUIBGSize * -0.5
		);
	}
	return rVal_;
}

//--------------------------------
void wallMgr::drawCloseUI()
{
	if (_isCloseUIVidible && _selectWallList)
	{
		_closeUI.draw(getCloseUIPos());
	}
}
#pragma endregion

#pragma region Select
//--------------------------------
void wallMgr::canSelect()
{
	_canSelect = true;
}

//--------------------------------
bool wallMgr::isCanSelect()
{
	return _canSelect;
}

//--------------------------------
void wallMgr::selectCheck(wallList* selectList)
{	
	auto backup_ = _selectWallList;
	if (_selectWallList)
	{		
		_selectWallList->disableInput();
		_selectWallList->enableInput();
		_selectWallList->deselect();
		textUIout();
		scrollUIout();
		closeUIout();
		_selectWallList = nullptr;
		_canSelect = true;
	}

	if (backup_ != selectList)
	{
		_selectWallList = selectList;
		selectList->disableInput();
		selectList->enableInput(true);

		updateTextUI(_selectWallList->getSelectPhotoID());
		_canSelect = false;
	}
}

#pragma endregion

#pragma region Input
//--------------------------------------
void wallMgr::enableInput()
{
	inputEventMgr::GetInstance()->registerInputEvent(this, eInputWallMgr);
}

//--------------------------------------
void wallMgr::disableInput()
{
	inputEventMgr::GetInstance()->unregisterInputEvent(this);
}

//--------------------------------------
void wallMgr::inputRelease(inputEventArgs e)
{
	if (_eWallState == eWallMainUI && _mainUI.isReady())
	{
		mainUIout();
	}
}

//--------------------------------------
ofRectangle wallMgr::getInputArea()
{
	return _wallRect;
}
#pragma endregion
