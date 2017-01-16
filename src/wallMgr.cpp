#include "wallMgr.h"

//--------------------------------
wallMgr::wallMgr()
	:_isSetup(false)
	, _selectWallList(nullptr)
	, _eState(eDeselect)
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
	setupSelect();

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
	updateSelect(delta);
	_mainUI.update(delta);

	selectAnimationCheck();
}

//--------------------------------
void wallMgr::draw(ofVec2f pos)
{
	setupCheck();

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos);
	ofVec2f drawPos_(0);
	for (auto& iter_ : _wallListMgr)
	{		
		int halfWidth_ = iter_->getBaseWidth() * 0.5f;
		drawPos_.x += halfWidth_;
		if (!iter_->getIsSelect())
		{
			iter_->draw(drawPos_);
		}
		drawPos_.x += halfWidth_;
	}

	drawUI();
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------
void wallMgr::drawSelect(ofVec2f pos)
{
	setupCheck();
	if (_selectWallList)
	{
		ofPushStyle();
		ofSetColor(255);
		ofPushMatrix();
		ofTranslate(pos);
		{
			_selectWallList->draw(ofVec2f(_animSelectPosX.getCurrentValue(), 0));
		}
		ofPopMatrix();
		ofPopStyle();
	}
}

//--------------------------------
void wallMgr::drawShadow(ofVec2f pos)
{
	setupCheck();
	if (_selectWallList)
	{
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(pos);
		{
			//Shadow
			imageRender::GetInstance()->drawImage(
				NAME_MGR::I_Gradient,
				ofVec2f(_animSelectPosX.getCurrentValue() + (cSelectShdowWidth * -0.5), 0),
				cSelectShdowWidth,
				cWindowHeight
			);
		}
		ofPopMatrix();
		ofPopStyle();
	}

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
void wallMgr::setupSelect()
{
	_animSelectPosX.setDuration(cSelectAnimLength);
	_animSelectPosX.setRepeatType(AnimRepeat::PLAY_ONCE);

}

//--------------------------------
void wallMgr::updateSelect(float delta)
{
	_animSelectPosX.update(delta);
}

//--------------------------------
void wallMgr::select(ofVec2f inputP, ofPtr<wallList>& selectList, ofVec2f pos)
{	
	if (_eState == eDeselect && !_selectWallList)
	{
		_selectWallList = selectList;
		_selectPosBackup = pos;

		float halfSelectWidth_ = cSelectWidth * 0.5f;
		int moveX_ = pos.x;
		if ((moveX_ + halfSelectWidth_) > _wallRect.getWidth())
		{
			moveX_ = _wallRect.getWidth() - halfSelectWidth_;
		}
		else if (moveX_ - halfSelectWidth_ < 0)
		{
			moveX_ = halfSelectWidth_;
		}

		_animSelectPosX.animateFromTo(pos.x, moveX_);
		_eState = eMoveFront;

		selectList->touchDown(inputP);
	}
	else if (_eState == eSelect && _selectWallList)
	{
		if (_selectWallList.get() != selectList.get())
		{
			_afterSelect = [=]() {
				ofPtr<wallList> nextWallList = selectList;
				ofVec2f nextP = pos;
				ofVec2f nextInput = inputP;
				select(inputP, nextWallList, nextP);
			};

			deselect();
		}
	}

}

//--------------------------------
void wallMgr::deselect()
{
	_animSelectPosX.animateTo(_selectPosBackup.x);
	_selectWallList->deselect();
	_eState = eMoveBack;
}

//--------------------------------
void wallMgr::selectAnimationCheck()
{
	switch (_eState)
	{
		case eMoveFront:
		{
			if (_animSelectPosX.hasFinishedAnimating() && _animSelectPosX.getPercentDone() == 1.0f)
			{
				_eState = eSelect;
			}
			break;
		}
		case eMoveBack:
		{
			if (_animSelectPosX.hasFinishedAnimating() && _animSelectPosX.getPercentDone() == 1.0f)
			{
				_eState = eDeselect;
				_selectWallList.reset();

				if (_afterSelect)
				{
					_afterSelect();
				}
			}
			break;
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
	if (_eWallState == eWallPhoto && _wallRect.inside(e))
	{
		ofVec2f inputPos_ = e;
		inputPos_.x -= _wallRect.x;

		int startPos_ = 0;
		for (auto& iter_ : _wallListMgr)
		{
			int width_ = iter_->getBaseWidth();
			if (inputPos_.x >= startPos_ && inputPos_.x < (startPos_ + width_))
			{
				ofVec2f selectPos_(startPos_ + width_ * 0.5f, 0);
							
				select(e, iter_, selectPos_);
				break;
			}
			startPos_ += width_;
		}
	}
	else if(_eWallState == eWallMainUI && _wallRect.inside(e))
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
