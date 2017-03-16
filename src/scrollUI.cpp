#include "scrollUI.h"
#include "wallMgr.h"

//---------------------------------
scrollUI::scrollUI()
	:_eState(eClose)
	, _isSetup(false)
	, _isZH(false)
	, _eCategory(ePhotoCategory_1)
	, _parent(nullptr)
	, _selectItemIdx(-1)
{}

//---------------------------------
void scrollUI::setup(wallMgr * parent)
{
	_parent = parent;
	_displayCanvas.allocate(cScrollUIWidth, cScrollUIHeight, GL_RGBA);

	_animRotate.setDuration(cScrollUIAnimDuration);
	_animRotate.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animRotate.reset(0.0);
	_eState = eClose;

	_isSetup = true;
}

//---------------------------------
void scrollUI::update(float delta)
{
	if (!_isSetup)
	{
		return;
	}
	_animRotate.update(delta);
	animStateCheck();
}

//---------------------------------
void scrollUI::draw(ofVec2f pos)
{
	if (!_isSetup)
	{
		return;
	}

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos);
	{
		_isRight ? drawRight() : drawLeft();
	}
	ofPopMatrix();
	ofPopStyle();
}

//---------------------------------
void scrollUI::updateScroll(ePhotoPrimaryCategory eCategory, bool isZH)
{
	_isZH = isZH;
	_eCategory = eCategory;
	updateItemList(eCategory);
	updateTitle(eCategory);
	updateCanvas();
}

//---------------------------------
void scrollUI::open(bool isRight, PHOTO_TYPE selectPhotoType)
{
	if (!_isSetup || _eState != eClose)
	{
		return;
	}

	if (selectPhotoType != -1 && _itemList.size() > selectPhotoType)
	{
		_itemList[selectPhotoType].setSelect(true);
		_selectItemIdx = selectPhotoType;
		updateCanvas();
	}

	_isRight = isRight;
	_animRotate.animateTo(1.0f);
	_eState = eUIIn;
}

//---------------------------------
void scrollUI::close()
{
	if (!_isSetup || _eState != eOpen)
	{
		return;
	}

	_animRotate.animateTo(0.0f);
	_eState = eUIOut;
}

//---------------------------------
void scrollUI::drawRight()
{
	ofPushMatrix();
	ofTranslate(cSelectWidth * 0.5, 0);
	ofRotateY(90.0 - _animRotate.getCurrentValue() * 90.0);
	{
		ofSetColor(255);
		_displayCanvas.draw(0, 0);
	}
	ofPopMatrix();
}

//---------------------------------
void scrollUI::drawLeft()
{
	ofPushMatrix();
	
	ofTranslate(cSelectWidth * -0.5, 0);
	ofRotateY(90.0 + _animRotate.getCurrentValue() * 90.0);
	ofScale(-1, 1);
	{
		ofSetColor(255);
		_displayCanvas.draw(-_displayCanvas.getWidth(), 0);
	}
	ofPopMatrix();
}

//---------------------------------
void scrollUI::updateCanvas()
{
	_displayCanvas.begin();
	{
		ofClear(255);
		drawItemList();	
		
		ofSetColor(getCategoryColor(_eCategory), cScrollUIAlpha);
		ofRect(0, 0, cScrollUIWidth, cScrollUIItemHeight);

		ofPushMatrix();
		ofTranslate(cScrollUIWidth * 0.5, cScrollUIItemHeight * 0.5);
		ofSetColor(255);
		_titleImg.draw(
			_titleImg.getWidth() * -0.5,
			_titleImg.getHeight() * -0.5
		);
		ofPopMatrix();
	}
	_displayCanvas.end();

}

//---------------------------------
void scrollUI::animStateCheck()
{
	switch (_eState)
	{
		case eUIIn:
		{
			if (_animRotate.hasFinishedAnimating() && _animRotate.getPercentDone() == 1.0f)
			{
				_eState = eOpen;
			}
			break;
		}
		case eUIOut:
		{
			if (_animRotate.hasFinishedAnimating() && _animRotate.getPercentDone() == 1.0f)
			{
				_eState = eClose;
				if (_selectItemIdx != -1)
				{
					_itemList[_selectItemIdx].setSelect(false);
					updateCanvas();
				}
				_selectItemIdx = -1;
			}
			break;
		}
	}
}

//---------------------------------
void scrollUI::updateTitle(ePhotoPrimaryCategory eCategory)
{
	_titleImg.clear();
	auto eFontType_ = (_isZH ? eFontMenuUITitle : eFontMenuUITitle);
	string titleName_ = dataHolder::GetInstance()->getCategoryName(eCategory, _isZH);

	auto titleRect_ = fontMgr::GetInstance()->getStringBoundingBox(eFontType_, titleName_, _isZH);
	ofFbo	_canvas;
	_canvas.allocate(titleRect_.getWidth(), titleRect_.getHeight(), GL_RGBA);

	_canvas.begin();
	{
		ofClear(255);

		ofSetColor(255);
		fontMgr::GetInstance()->drawString(eFontType_, titleName_, ofVec2f(-titleRect_.x, -titleRect_.y), _isZH);
	}
	_canvas.end();

	ofPixels pixel_;
	_canvas.readToPixels(pixel_);
	_titleImg.setFromPixels(pixel_);
}

//---------------------------------
ofVec2f scrollUI::getDrawPos()
{
	auto rVal_ = _parent->getWallRectPos() + _parent->getScrollUIPos();
	rVal_.x += (_isRight ? cSelectWidth * 0.5 : -(cSelectWidth * 0.5 + cScrollUIWidth));
	return rVal_;
}

#pragma region item

#pragma region CLASS itemUnit
//---------------------------------
void scrollUI::itemUnit::setSelect(bool val)
{
	_isSelect = val;
}

//---------------------------------
void scrollUI::itemUnit::draw(ofVec2f pos, ePhotoPrimaryCategory eCategory, bool isZH)
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos.x + cScrollUIWidth * 0.5, pos.y + cScrollUIItemHeight * 0.5);
	{	
		//Backplace
		_isSelect ? ofSetColor(cScrollUIItemSelectColor) : ofSetColor(cScrollUIItemColor);
		ofRect(cScrollUIWidth * -0.5, cScrollUIItemHeight * -0.5, cScrollUIWidth, cScrollUIItemHeight);

		//Text
		auto eFontType_ = (isZH ? eFontMenuUIContext : eFontMenuUIContext);
		string itemName_ = dataHolder::GetInstance()->getTypeName(eCategory, _photoType, isZH);
		auto bounding_ = fontMgr::GetInstance()->getStringBoundingBox(eFontType_, itemName_, isZH);
		ofSetColor(255);
		fontMgr::GetInstance()->drawString(eFontType_, itemName_, ofVec2f(bounding_.getWidth() * -0.5, bounding_.getHeight() * 0.4), isZH);

	}
	ofPopMatrix();
	ofPopStyle();
}

//---------------------------------
PHOTO_TYPE scrollUI::itemUnit::getPhotoType()
{
	return _photoType;
}
#pragma endregion
//---------------------------------
void scrollUI::drawItemList()
{
	int startPos_ = _itemListDrawY + cScrollUIItemStartPosY;
	ofPushStyle();
	{
		for (auto& iter_ : _itemList)
		{
			if (startPos_ > cScrollUIHeight)
			{
				break;
			}
			iter_.draw(ofVec2f(0, startPos_), _eCategory, _isZH);
			startPos_ += cScrollUIItemHeight;
		}
	}
	ofPopStyle();
}

//---------------------------------
void scrollUI::updateItemList(ePhotoPrimaryCategory eCategory)
{
	_itemList.clear();

	auto typeList_ = dataHolder::GetInstance()->getType(eCategory);
	for (auto& iter_ : typeList_)
	{
		itemUnit unit_(iter_);
		_itemList.push_back(unit_);
	}

	_itemListDrawY = 0;
	int temp_ = -( int(_itemList.size() * cScrollUIItemHeight) - (cScrollUIHeight - cScrollUIItemHeight));
	_itemListDrawYMin = MIN(0, temp_);
}

//---------------------------------
bool scrollUI::moveDrawPosY(int delta)
{
	int afterMove_ = _itemListDrawY + delta;
	afterMove_ = MAX(MIN(afterMove_, 0), _itemListDrawYMin);
	
	
	if (_itemListDrawY == afterMove_)
	{
		return false;
	}
	else
	{
		_itemListDrawY = afterMove_;
		return true;
	}
}

//---------------------------------
int scrollUI::getSelectIndex(ofVec2f pos)
{
	int rVal_ = -1;
	auto localPos_ = pos - getDrawPos();
	localPos_.y -= cScrollUIItemStartPosY;
	if (localPos_.x > 0 && localPos_.x < cScrollUIWidth &&
		localPos_.y > 0 && localPos_.y < cScrollUIHeight)
	{
		float dist_ = localPos_.y - _itemListDrawY;
		rVal_ = static_cast<int>(dist_ / cScrollUIItemHeight);

		if (rVal_ < 0 || rVal_ > _itemList.size())
		{
			ofLog(OF_LOG_ERROR, "[scrollUI::getSelectIndex]Wrong compute");
			rVal_ = -1;
		}
	}

	return rVal_;
}

#pragma endregion

#pragma region Input

//---------------------------------
void scrollUI::enableInput(bool isSelect)
{
	inputEventMgr::GetInstance()->registerInputEvent(this, eInputSystemLevel::eInputScrollUI);
}

//---------------------------------
void scrollUI::disableInput()
{
	inputEventMgr::GetInstance()->unregisterInputEvent(this);
}

//---------------------------------
void scrollUI::inputPress(inputEventArgs e)
{
}

//---------------------------------
void scrollUI::inputDrag(inputEventArgs e)
{
	if (moveDrawPosY(e.delta.y))
	{
		updateCanvas();
	}
}

//---------------------------------
void scrollUI::inputRelease(inputEventArgs e)
{
	if (e.holdTime <= cInputHoldLimit && abs(e.diffPos.y) < cInputTriggerDiffLimit)
	{
		int newSelectIdx_ = getSelectIndex(e.pos);
		if (newSelectIdx_ != -1 && _selectItemIdx != newSelectIdx_)
		{
			if (_selectItemIdx != -1)
			{
				_itemList[_selectItemIdx].setSelect(false);
			}
			_itemList[newSelectIdx_].setSelect(true);
			_selectItemIdx = newSelectIdx_;

			updateCanvas();

			_parent->selectType(_itemList[newSelectIdx_].getPhotoType());
		}
	}
}

//---------------------------------
ofRectangle scrollUI::getInputArea()
{
	ofRectangle rVal_;
	if (_isSetup && _eState == eOpen)
	{
		rVal_.set(getDrawPos(), cScrollUIWidth, cScrollUIHeight);
	}
	return rVal_;
}
#pragma endregion

