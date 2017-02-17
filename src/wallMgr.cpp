#include "wallMgr.h"


//--------------------------------
wallMgr::wallMgr()
	:_isSetup(false)
	, _selectWallList(nullptr)
	, _eWallState(eWallIdle)
	, _isDisplayZH(true)
	, _canSelect(true)
{
	
}

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
}

//--------------------------------
void wallMgr::end()
{
	if (_eCategory != _backupCategory)
	{
		changeCategory(_backupCategory);
	}
	_isDisplayZH = true;
	_mainUI.end();	

	disableWallListInput();
	disableInput();
	_eWallState = eWallIdle;
}

//--------------------------------
void wallMgr::addWallList(int width)
{
	setupCheck();
	ofRectangle	rect_(getListTotalWidth(), 0, width, _wallRect.getHeight());
	_wallListMgr.push_back(ofPtr<wallList>(new wallList(this, _eCategory, rect_)));
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
}

//--------------------------------
void wallMgr::updateTextUI(int photoID)
{
	//TODO
	string titleZH_ = "標題:" + ofToString(photoID);
	string titleEN_ = "TITLE:" + ofToString(photoID);
	string msgZH_ = "零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九";
	string msgEN_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ012345678901234567890123456789";
	
	if (_isDisplayZH)
	{
		_textUI.updateText(titleZH_, msgZH_, _isDisplayZH);
	}
	else
	{
		_textUI.updateText(titleEN_, msgEN_, _isDisplayZH);
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
	_scrollUI.open();
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
			_selectWallList->getDrawPosX() + cSelectWidth * 0.5,
			_selectWallList->getSelectTopPosY()
		);
	}
	return rVal_;
}

//--------------------------------
void wallMgr::selectType(PHOTO_TYPE type)
{
	if (_selectWallList)
	{
		setTextUIVisible(false);
		setScrollUIVisible(false);
		_selectWallList->selectType(type);
	}
}

//--------------------------------
void wallMgr::drawScrollUI()
{
	if (_isTextUIVisible && _selectWallList)
	{	
		_scrollUI.draw(getScrollUIPos());
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
		_selectWallList = nullptr;
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
	if (_eWallState == eWallMainUI)
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
