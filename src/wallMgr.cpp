#include "wallMgr.h"

//--------------------------------
wallMgr::wallMgr()
	:_isSetup(false)
	, _selectWallList(nullptr)
	, _eWallState(eWallIdle)
	
{
	
}

//--------------------------------
wallMgr::~wallMgr()
{
	_wallListMgr.clear();
}

//--------------------------------
void wallMgr::setup(ePhotoPrimaryCategory category, ofRectangle wallRect)
{
	_eCategory = category;
	_wallRect = wallRect;

	_mainUI.setup(this, "config/mainUIText.xml", category);

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
		if (!iter_->getIsSelect())
		{
			iter_->draw();
		}	
	}

	drawUI();
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
		if (iter_->getIsSelect())
		{
			iter_->draw();
		}
	}

	drawUI();
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
				ofVec2f(iter_->getListPosX() - cSelectShdowWidth * 0.5f, 0),
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
	int posX_ = getListTotalWidth();
	ofRectangle	rect_(posX_, 0, width, _wallRect.getHeight());
	_wallListMgr.push_back(ofPtr<wallList>(new wallList(this, _eCategory, rect_)));
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

#pragma region UI
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
void wallMgr::drawUI()
{
	_mainUI.draw(ofVec2f(cPhotoWallCategoryWidth * 0.5, cWindowHeight * 0.5));
}
#pragma endregion


#pragma region Select

//--------------------------------
void wallMgr::selectCheck(wallList* selectList)
{	
	if (!_selectWallList)
	{
		_selectWallList = selectList;
		selectList->disableInput();
		selectList->enableInput(true);
	}
	else
	{	
		if (_selectWallList != selectList)
		{
			_selectWallList->disableInput();
			_selectWallList->enableInput();
			_selectWallList->deselect();

			_selectWallList = selectList;
			selectList->disableInput();
			selectList->enableInput(true);
		}
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

