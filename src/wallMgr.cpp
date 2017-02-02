#include "wallMgr.h"


//--------------------------------
wallMgr::wallMgr()
	:_isSetup(false)
	, _selectWallList(nullptr)
	, _eWallState(eWallIdle)
	, _isDisplayZH(true)
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
	_eCategory = category;
	_wallRect = wallRect;

	_mainUI.setup(this, ofVec2f(cPhotoWallCategoryWidth * 0.5, cWindowHeight * 0.5), "config/mainUIText.xml", category);
	_textUI.setup(this);
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
	ofVec2f drawPos_(0);
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
	_mainUI.open();
	_eWallState = eWallMainUI;
	enableInput();
}

//--------------------------------
void wallMgr::mainUIout()
{
	_mainUI.close();
	_eWallState = eWallPhoto;
	disableInput();
	enableWallListInput();
}

//--------------------------------
void wallMgr::changeLanguage()
{
	_isDisplayZH ^= true;
}

#pragma endregion

#pragma region textUI
//--------------------------------
void wallMgr::textUIin()
{
	_isVisible = true;
	_textUI.open();
	_textUI.enableInput();
}

//--------------------------------
void wallMgr::textUIout()
{
	_isVisible = false;
	_textUI.close();
	_textUI.disableInput();
}

//--------------------------------
void wallMgr::setTextUIVisible(bool val)
{
	_isVisible = val;
}

//--------------------------------
void wallMgr::updateTextUI(int photoID)
{
	//TODO
	string titleZH_ = "標題:" + ofToString(photoID);
	string titleEN_ = "TITLE:" + ofToString(photoID);
	string msgZH_ = "零一二三四五六七八九零一二三四五六七八九零一二三四五六七八九";
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
void wallMgr::drawTextUI()
{
	if (_isVisible && _selectWallList)
	{
		_textUI.draw(ofVec2f(_selectWallList->getDrawPosX(), _selectWallList->getSelectBottomPosY()));
	}
	
}
#pragma endregion

#pragma region Select

//--------------------------------
void wallMgr::selectCheck(wallList* selectList)
{	
	if (_selectWallList)
	{		
		_selectWallList->disableInput();
		_selectWallList->enableInput();
		_selectWallList->deselect();
		textUIout();
	}

	if (_selectWallList != selectList)
	{
		_selectWallList = selectList;
		selectList->disableInput();
		selectList->enableInput(true);

		updateTextUI(_selectWallList->getSelectPhotoID());
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

