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

	_mainUI.setup("config/mainUIText.xml", category);

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

#pragma region UI
//--------------------------------
void wallMgr::mainUIin()
{
	_mainUI.open();
	_eWallState = eWallMainUI;
}

//--------------------------------
void wallMgr::mainUIout()
{
	_mainUI.close();
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
	}
	else
	{	
		if (_selectWallList != selectList)
		{
			_selectWallList->deselect();
			_selectWallList = selectList;
		}
	}
}
#pragma endregion

#pragma region Input
#ifdef USE_MOUSE
//--------------------------------
void wallMgr::mouseDragged(ofMouseEventArgs& e)
{
}

//--------------------------------
void wallMgr::mousePressed(ofMouseEventArgs& e)
{
	if(_eWallState == eWallMainUI && _wallRect.inside(e))
	{
		mainUIout();
		_eWallState = eWallPhoto;
	}
}

//--------------------------------
void wallMgr::mouseReleased(ofMouseEventArgs& e)
{
}

#else
//--------------------------------
void wallMgr::touchDown(ofTouchEventArgs& e)
{
}

//--------------------------------
void wallMgr::touchMoved(ofTouchEventArgs& e)
{
}

//--------------------------------
void wallMgr::touchUp(ofTouchEventArgs& e)
{
}
#endif // USE_MOUSE
//--------------------------------
void wallMgr::enableInput()
{
#ifdef USE_MOUSE
	ofRegisterMouseEvents(this);
#else
	ofxRegisterWinTouchEvents(this);
#endif // USE_MOUSE
}

//--------------------------------
void wallMgr::disableInput()
{
#ifdef USE_MOUSE
	ofUnregisterMouseEvents(this);
#else
	ofxUnregisterWinTouchEvents(this);
#endif // USE_MOUSE
}
#pragma endregion

