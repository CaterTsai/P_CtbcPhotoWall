#include "wallList.h"

//--------------------------------------
wallList::wallList(ePhotoPrimaryCategory eCategroy, int width, int height)
	:_baseWidth(width)
	,_displayheight(height)
	, _eState(eDeselect)
	,_centerUnitPos(0.0)
	, _eCategroy(eCategroy)
{
	setupAnimation(width);
	addWallUnits();
	resetCenter();
	
}

//--------------------------------------
wallList::~wallList()
{
	_wallUnitList.clear();
}

//--------------------------------------
void wallList::update(float delta)
{
	updateAnimation(delta);
	updateWallUnit(delta);
	updateCenter(delta);

	checkAnimationState();
}

//--------------------------------------
void wallList::draw(ofVec2f pos)
{
	ofPushMatrix();
	ofTranslate(pos);

	drawWallUnitUp();
	drawWallUnitDown();

	ofPopMatrix();
}

//--------------------------------------
float wallList::getBaseWidth()
{
	return _baseWidth;
}

#pragma region Center
//--------------------------------------
void wallList::touchDown(ofVec2f & pos)
{
	if (!getIsSelect())
	{
		select(pos);
	}
}

//--------------------------------------
void wallList::touchMoved(ofVec2f & delta)
{
}

//--------------------------------------
void wallList::touchUp(ofVec2f & pos)      
{
}

//--------------------------------------
void wallList::resetCenter()
{
	_centerUnitPos.set(0);
	_centerVec.set(0.0f, ofRandom(-1, 1)>0?ofRandom(30.0, 80.0):ofRandom(-80.0, -30.0));
	
}

//--------------------------------------
void wallList::updateCenter(float delta)
{
	if (getIsSelect())
	{
		if (_eState == eZoomIn || _eState == eZoomOut)
		{
			_centerUnitPos.y = getCenterUnitPosYFromSelect();
		}
	}
	else
	{
		float diff_ = _centerVec.y * delta;
		int height_ = (*_wallUnitList.begin())->getHeight();
		_centerUnitPos.y += diff_;

		if (_centerUnitPos.y > _wallTotalHeight)
		{
			_centerUnitPos.y = _centerUnitPos.y - _wallTotalHeight;
		}
		else if (_centerUnitPos.y < -height_)
		{
			_centerUnitPos.y = _wallTotalHeight + _centerUnitPos.y;
		}
	}
}

//--------------------------------------
int wallList::touchCheckUp(float diff, ofVec2f & pos)
{
	int posY_ = 0;
	
	int idx_ = _wallUnitList.size() - 1;
	for (auto& iter_ = _wallUnitList.rbegin(); iter_ != _wallUnitList.rend(); iter_++)
	{
		int height_ = (*iter_)->getHeight();
		
		if (diff >= posY_ && diff < (posY_ + height_))
		{
			(*iter_)->onclick(pos);
			_selectPos = _centerUnitPos;
			_selectPos.y -= (posY_ + height_);
			break;
		}
		posY_ += height_;
		idx_--;
	}
	return idx_;
}

//--------------------------------------
int wallList::touchCheckDown(float diff, ofVec2f & pos)
{
	int posY_ = 0;
	int idx_ = 0;
	for (auto& iter_ : _wallUnitList)
	{
		int height_ = iter_->getHeight();
		
		if (diff >= posY_ && diff < (posY_ + height_))
		{
			_selectPos = _centerUnitPos;
			_selectPos.y += posY_;
			break;
		}
		posY_ += height_;
		idx_++;
	}
	return idx_;
}

//--------------------------------------
int wallList::getCenterUnitPosYFromSelect()
{
	//TODO : Performance Point
	int returnVal_ = 0;
	if (_selectUp)
	{
		for (int idx_ = _selectId; idx_ < _wallUnitList.size(); idx_++)
		{
			returnVal_ += _wallUnitList[idx_]->getHeight();
		}
	}
	else
	{
		for (int idx_ = 0; idx_ < _selectId; idx_++)
		{
			returnVal_ -= _wallUnitList[idx_]->getHeight();
		}
	}
	return (_selectPos.y + returnVal_);
}

#pragma endregion

#pragma region animation
//--------------------------------------
bool wallList::getIsSelect()
{
	return (_eState != eDeselect);
}

//--------------------------------------
bool wallList::select(ofVec2f& pos)
{
	if (_eState == eDeselect)
	{	
		//wall unit check
		int diff_ = pos.y - _centerUnitPos.y;
		ofVec2f centerMove_;
		if (diff_ >= 0)
		{
			_selectUp = false;
			_selectId = touchCheckDown(diff_, pos);
		}
		else
		{
			_selectUp = true;
			diff_ = abs(diff_);
			_selectId = touchCheckUp(diff_, pos);
		}
		_centerUnitPosBackup = _centerUnitPos;

		//trigger animation
		_eState = eZoomIn;
		_animDrawWidth.animateTo(cSelectWidth);
		_centerVec.set(0);
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------
bool wallList::deselect()
{
	if (_eState == eSelect)
	{
		_eState = eZoomOut;
		_animDrawWidth.animateTo(_baseWidth);
		
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------
void wallList::setupAnimation(int width)
{
	_animDrawWidth.setDuration(cSelectAnimLength);
	_animDrawWidth.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animDrawWidth.reset(width);
}

//--------------------------------------
void wallList::updateAnimation(float delta)
{
	_animDrawWidth.update(delta);

}

//--------------------------------------
void wallList::checkAnimationState()
{
	switch (_eState)
	{
		case eZoomIn:
		{
			if (_animDrawWidth.hasFinishedAnimating() && _animDrawWidth.getPercentDone() == 1.0)
			{
				_eState = eSelect;
			}
			break;
		}
		case eZoomOut:
		{
			if (_animDrawWidth.hasFinishedAnimating() && _animDrawWidth.getPercentDone() == 1.0)
			{
				_eState = eDeselect;
				_centerVec.set(0.0f, ofRandom(-1, 1)>0 ? ofRandom(30.0, 80.0) : ofRandom(-80.0, -30.0));
			}
			break;
		}
		default:
		{
			break;
		}
	}
}
#pragma endregion

#pragma region WallUnit
//--------------------------------------
void wallList::setupWallUnit()
{
	for (auto& Iter_ : _wallUnitList)
	{
		Iter_->update(0, _baseWidth);
	}
}

//--------------------------------------
void wallList::updateWallUnit(float delta)
{
	for (auto& Iter_ : _wallUnitList)
	{
		Iter_->update(delta, _animDrawWidth.getCurrentValue());
	}
	updateWallTotalHeight();
}
//--------------------------------------
void wallList::drawWallUnitUp()
{
	ofPushStyle();
	ofVec2f wallPos_ = _centerUnitPos;
	wallPos_.x -= _animDrawWidth.getCurrentValue() * 0.5f;

	for (auto iter_ = _wallUnitList.rbegin(); iter_ != _wallUnitList.rend(); iter_++)
	{
		if (wallPos_.y < 0)
		{
			break;
		}

		wallPos_.y -= (*iter_)->getHeight();
		if (wallPos_.y >= _displayheight)
		{
			continue;
		}
		(*iter_)->draw(wallPos_);
	}

	ofPopStyle();
}

//--------------------------------------
void wallList::drawWallUnitDown()
{
	ofPushStyle();
	ofVec2f wallPos_ = _centerUnitPos;
	wallPos_.x -= _animDrawWidth.getCurrentValue() * 0.5f;
	for (auto& iter_ : _wallUnitList)
	{	
		iter_->draw(wallPos_);
		wallPos_.y += iter_->getHeight();

		if (wallPos_.y > _displayheight)
		{
			break;
		}
	}

	ofPopStyle();
}

//--------------------------------------
void wallList::addWallUnit(ofPtr<wallUnit> newUnil)
{
	_wallUnitList.push_back(newUnil);
}

//--------------------------------------
void wallList::addWallUnits()
{
	auto photoIDList_ = dataHolder::GetInstance()->getPhotoID(_eCategroy);

	random_shuffle(photoIDList_.begin(), photoIDList_.end());

	int idx_ = 0;
	for (auto& iter_ : photoIDList_)
	{
		auto photoHeader_ = dataHolder::GetInstance()->getPhotoHeader(iter_);
		addWallUnit(ofPtr<wallUnit>(new photoUnit(photoHeader_, _animDrawWidth.getCurrentValue())));

		idx_++;
		if (idx_ > cDefaultPhotoListNum)
		{
			break;
		}
	}
	updateWallTotalHeight();
}

//--------------------------------------
void wallList::addWallUnits(int type)
{
}

//--------------------------------------
void wallList::removeWallUnits()
{
	_wallUnitList.clear();
}

//--------------------------------------
void wallList::updateWallTotalHeight()
{
	_wallTotalHeight = 0;

	for (auto& iter_ : _wallUnitList)
	{
		_wallTotalHeight += iter_->getHeight();
	}
}

#pragma endregion

