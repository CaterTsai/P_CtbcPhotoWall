#include "wallMgr.h"

//--------------------------------
wallMgr::wallMgr()
	:_isSetup(false)
	, _selectWallList(nullptr)
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
	enableInput();

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
}

//--------------------------------
void wallMgr::draw(ofVec2f pos)
{
	setupCheck();

	ofPushMatrix();
	ofTranslate(pos);
	ofVec2f drawPos_(0);
	for (auto& iter_ : _wallListMgr)
	{		
		if (!iter_->getIsSelect())
		{
			iter_->draw(drawPos_);
		}
		drawPos_.x += iter_->getBaseWidth();
	}

	if (_selectWallList)
	{
		_selectWallList->draw(ofVec2f(0, 0));
	}
	ofPopMatrix();

}

//--------------------------------
void wallMgr::addWallList(int width)
{
	setupCheck();
	_wallListMgr.push_back(ofPtr<wallList>(new wallList(_eCategory, width, _wallRect.getHeight())));
}

//--------------------------------
void wallMgr::setupCheck()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("setupCheck : need setup"));
	}
}
#pragma region Input
#ifdef USE_MOUSE
//--------------------------------
void wallMgr::mouseDragged(ofMouseEventArgs& e)
{
}

//--------------------------------
void wallMgr::mousePressed(ofMouseEventArgs& e)
{
	if (_wallRect.inside(e))
	{
		ofVec2f inputPos_ = e;
		inputPos_.x -= _wallRect.x;

		int startPos_ = 0;
		for (auto& iter_ : _wallListMgr)
		{
			if (inputPos_.x >= startPos_ && inputPos_.x < (startPos_ + iter_->getBaseWidth()))
			{
				iter_->touchDown(e);
				_selectWallList = iter_;
				break;
			}
			startPos_ += iter_->getBaseWidth();
		}
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
