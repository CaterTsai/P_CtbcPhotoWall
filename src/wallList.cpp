#include "wallList.h"
#include "wallMgr.h"
//--------------------------------------
wallList::wallList(wallMgr* parent, ePhotoPrimaryCategory eCategroy, ofRectangle drawArea)
	:_baseArea(drawArea)
	,_eSelectState(eDeselect)
	,_centerUnitPos(0.0)
	, _eCategroy(eCategroy)
	,_parent(parent)
{
	setupAnimation(drawArea.getCenter().x, _baseArea.getWidth());
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
	getIsDeselect()? updateCenter(delta): updateSelectCenter(delta);

	checkSelectState();
	checkSelectDrapState();
}

//--------------------------------------
void wallList::draw()
{
	ofPushMatrix();
	ofTranslate(_animDrawPosX.getCurrentValue(), 0);

	drawWallUnitUp();
	drawWallUnitDown();

	ofPopMatrix();
}

//--------------------------------------
float wallList::getBaseWidth()
{
	return _baseArea.getWidth();
}

//--------------------------------------
int wallList::getListPosX()
{
	return _baseArea.getCenter().x;
}

#pragma region Center

//--------------------------------------
void wallList::resetCenter()
{
	_centerUnitPos.set(0);
	_centerVec.set(0.0f, ofRandom(-1, 1)>0?ofRandom(20.0, 60.0):ofRandom(-60.0, -20.0));
	_centerBaseVec = _centerVec;
}

//--------------------------------------
void wallList::updateCenter(float delta)
{

	if (abs(_centerVec.y) > abs(_centerBaseVec.y))
	{
		_centerVec.y *= 0.96;
	}

	float diff_ = _centerVec.y * delta;
	int height_ = (*_wallUnitList.begin())->getHeight();
	_centerUnitPos.y += diff_;

	fixCenterUnitPos();
}

//--------------------------------------
void wallList::updateSelectCenter(float delta)
{
	if (_eSelectState == eZoomIn || _eSelectState == eZoomOut)
	{
		_centerUnitPos.y = getCenterUnitPosYFromSelect();
	}
	else
	{
		if (_eSelectDrapState == eMove)
		{
			_centerUnitPos.y = _animMoveSelect.getCurrentValue();
		}
	}
}

//--------------------------------------
wallUnitInfo wallList::foundWallUnit(ofVec2f pos, bool nearest)
{
	int diff_ = pos.y - _centerUnitPos.y;
	if (diff_ >= 0)
	{
		return nearest ? foundUnitDownNearest(diff_) : foundUnitDown(diff_);
	}
	else
	{
		return nearest ? foundUnitUpNearest(abs(diff_)) : foundUnitUp(abs(diff_));
	}
}

//--------------------------------------
wallUnitInfo wallList::foundUnitUp(float diff)
{
	wallUnitInfo val_;
	int posY_ = 0;
	
	int idx_ = _wallUnitList.size() - 1;
	for (auto& iter_ = _wallUnitList.rbegin(); iter_ != _wallUnitList.rend(); iter_++)
	{
		int height_ = (*iter_)->getHeight();
		
		if (diff >= posY_ && diff < (posY_ + height_))
		{
			val_.pos = _centerUnitPos;
			val_.pos.y -= (posY_ + height_);
			break;
		}
		posY_ += height_;
		idx_--;
	}
	val_.id = idx_;
	return val_;
}

//--------------------------------------
wallUnitInfo wallList::foundUnitDown(float diff)
{
	wallUnitInfo val_;
	int posY_ = 0;
	int idx_ = 0;
	for (auto& iter_ : _wallUnitList)
	{
		int height_ = iter_->getHeight();
		
		if (diff >= posY_ && diff < (posY_ + height_))
		{
			val_.pos = _centerUnitPos;
			val_.pos.y += posY_;
			break;
		}
		posY_ += height_;
		idx_++;
	}
	val_.id = idx_;
	return val_;
}

//--------------------------------------
wallUnitInfo wallList::foundUnitUpNearest(float diff)
{
	wallUnitInfo val_;
	int posY_ = 0;
	int minDist_ = diff;
	int minID_ = 0;

	int idx_ = _wallUnitList.size() - 1;
	for (auto& iter_ = _wallUnitList.rbegin(); iter_ != _wallUnitList.rend(); iter_++)
	{
		int height_ = (*iter_)->getHeight();
		int dist_ = abs(diff - (posY_ + height_));
		if (dist_ > minDist_)
		{
			val_.pos = _centerUnitPos;
			val_.pos.y -= posY_;
			val_.id = minID_;
			break;
		}
		else
		{
			minDist_ = dist_;
			minID_ = idx_;
		}
		posY_ += height_;
		idx_--;
	}
	return val_;
}

//--------------------------------------
wallUnitInfo wallList::foundUnitDownNearest(float diff)
{
	wallUnitInfo val_;
	int minDist_ = INT_MAX;
	int minID_ = -1;

	int posY_ = 0;
	int idx_ = 0;
	int height_ = 0;
	for (auto& iter_ : _wallUnitList)
	{	
		int dist_ = abs(diff - posY_);
		if (dist_ > minDist_)
		{
			val_.id = minID_;
			val_.pos = _centerUnitPos;
			val_.pos.y += (posY_ - height_);
			break;
		}
		else
		{
			minDist_ = dist_;
			minID_ = idx_;
		}

		height_ = iter_->getHeight();
		posY_ += height_;
		
		idx_++;
	}
	
	return val_;
}

//--------------------------------------
int wallList::getCenterUnitPosYFromSelect()
{
	//TODO : Performance Point
	int returnVal_ = 0;
	if (_selectWallUnit.pos.y < _centerUnitPos.y)
	{
		for (int idx_ = _selectWallUnit.id; idx_ < _wallUnitList.size(); idx_++)
		{
			returnVal_ += _wallUnitList[idx_]->getHeight();
		}
	}
	else
	{
		for (int idx_ = 0; idx_ < _selectWallUnit.id; idx_++)
		{
			returnVal_ -= _wallUnitList[idx_]->getHeight();
		}
	}
	return (_selectWallUnit.pos.y + returnVal_);
}

//--------------------------------------
void wallList::fixCenterUnitPos()
{
	if (_centerUnitPos.y > _wallTotalHeight)
	{
		_centerUnitPos.y = _centerUnitPos.y - _wallTotalHeight;
	}
	else if (_centerUnitPos.y < -(*_wallUnitList.begin())->getHeight())
	{
		_centerUnitPos.y = _wallTotalHeight + _centerUnitPos.y;
	}
}

#pragma endregion

#pragma region animation
//--------------------------------------
bool wallList::getIsSelect()
{
	return (_eSelectState == eSelect);
}

//--------------------------------------
bool wallList::getIsDeselect()
{
	return (_eSelectState == eDeselect);
}

//--------------------------------------
bool wallList::select(ofVec2f& pos)
{
	if (_eSelectState == eDeselect)
	{	
		_selectWallUnit = foundWallUnit(pos);
		_wallUnitList[_selectWallUnit.id]->setClick(true, pos);

		//trigger animation
		_eSelectState = eZoomIn;

		_animDrawPosX.animateTo(getAnimMoveX());
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
	if (_eSelectState == eSelect)
	{
		_eSelectState = eZoomOut;
		_animDrawPosX.animateTo(getListPosX());
		_animDrawWidth.animateTo(_baseArea.getWidth());
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------
void wallList::setupAnimation(int posX, int width)
{
	_animDrawPosX.setDuration(cSelectAnimLength);
	_animDrawPosX.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animDrawPosX.reset(posX);

	_animDrawWidth.setDuration(cSelectAnimLength);
	_animDrawWidth.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animDrawWidth.reset(width);

	_animMoveSelect.setDuration(0.2);
	_animMoveSelect.setRepeatType(AnimRepeat::PLAY_ONCE);
}

//--------------------------------------
void wallList::updateAnimation(float delta)
{
	_animDrawWidth.update(delta);
	_animDrawPosX.update(delta);
	_animMoveSelect.update(delta);
}

//--------------------------------------
void wallList::checkSelectState()
{
	switch (_eSelectState)
	{
		case eZoomIn:
		{
			if (_animDrawWidth.hasFinishedAnimating() && _animDrawWidth.getPercentDone() == 1.0)
			{
				_eSelectState = eSelect;
			}
			break;
		}
		case eZoomOut:
		{
			if (_animDrawWidth.hasFinishedAnimating() && _animDrawWidth.getPercentDone() == 1.0)
			{
				_wallUnitList[_selectWallUnit.id]->setClick(false);
				_eSelectState = eDeselect;
				_selectWallUnit.id = -1;
				_centerVec.set(0.0f, ofRandom(-1, 1)>0 ? ofRandom(30.0, 60.0) : ofRandom(-60.0, -30.0));
				_centerBaseVec = _centerVec;
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

//--------------------------------------
void wallList::checkSelectDrapState()
{
	if (_eSelectDrapState == eMove && _animMoveSelect.hasFinishedAnimating() && _animMoveSelect.getPercentDone() == 1.0)
	{
		_eSelectDrapState = eStable;
		_wallUnitList[_selectWallUnit.id]->setClick(true);
	}
}

//--------------------------------------
int wallList::getAnimMoveX()
{
	float halfSelectWidth_ = cSelectWidth * 0.5f;
	int moveX_ = getListPosX();
	if ((moveX_ + halfSelectWidth_) > _parent->getWallRectWidth())
	{
		moveX_ = _parent->getWallRectWidth() - halfSelectWidth_;
	}
	else if (moveX_ - halfSelectWidth_ < 0)
	{
		moveX_ = halfSelectWidth_;
	}
	return moveX_;
}

//--------------------------------------
void wallList::fitSelectPos()
{
	auto nearestWallUnit_ = foundWallUnit(_selectWallUnit.pos, true);
	_selectWallUnit.id = nearestWallUnit_.id;
	float diffY_ = nearestWallUnit_.pos.y - _selectWallUnit.pos.y;

	_animMoveSelect.reset(_centerUnitPos.y);
	_animMoveSelect.animateTo(_centerUnitPos.y - diffY_);
	_eSelectDrapState = eMove;
}

#pragma endregion

#pragma region WallUnit
//--------------------------------------
void wallList::setupWallUnit()
{
	for (auto& Iter_ : _wallUnitList)
	{
		Iter_->update(0, _baseArea.getWidth());
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

	int idx_ = _wallUnitList.size() - 1;
	for (auto iter_ = _wallUnitList.rbegin(); iter_ != _wallUnitList.rend(); iter_++)
	{
		if (wallPos_.y < 0)
		{
			break;
		}

		wallPos_.y -= (*iter_)->getHeight();
		if (wallPos_.y >= _baseArea.getHeight())
		{
			continue;
		}
		(*iter_)->draw(wallPos_, !getIsDeselect());
		
		idx_--;
	}

	ofPopStyle();
}

//--------------------------------------
void wallList::drawWallUnitDown()
{
	ofPushStyle();
	ofVec2f wallPos_ = _centerUnitPos;
	wallPos_.x -= _animDrawWidth.getCurrentValue() * 0.5f;
	int idx_ = 0;
	for (auto& iter_ : _wallUnitList)
	{	
		iter_->draw(wallPos_, !getIsDeselect());
		
		wallPos_.y += iter_->getHeight();

		if (wallPos_.y > _baseArea.getHeight())
		{
			break;
		}
		idx_++;
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

#pragma region Input
//--------------------------------------
void wallList::enableInput(bool isSelect)
{
	if (isSelect)
	{
		inputEventMgr::GetInstance()->registerInputEvent(this, eInputWallSelectList);
	}
	else
	{
		inputEventMgr::GetInstance()->registerInputEvent(this, eInputWallList);
	}
}

//--------------------------------------
void wallList::disableInput()
{
	inputEventMgr::GetInstance()->unregisterInputEvent(this);
}

//--------------------------------------
void wallList::inputPress(inputEventArgs e)
{
	_centerVec.set(0);

	if (getIsSelect())
	{
		
		
	}
}

//--------------------------------------
void wallList::inputDrag(inputEventArgs e)
{
	if (_eSelectState != eZoomIn && _eSelectState != eZoomOut)
	{
		_centerUnitPos.y += e.delta.y;

		if (getIsSelect() && abs(e.diffPos.y) > cInputTriggerDiffLimit && _wallUnitList[_selectWallUnit.id]->getClick())
		{
			_wallUnitList[_selectWallUnit.id]->setClick(false);
			_eSelectDrapState = eDrap;
		}
	}
}

//--------------------------------------
void wallList::inputRelease(inputEventArgs e)
{
	if (e.holdTime <= cInputHoldLimit && abs(e.diffPos.y) < cInputTriggerDiffLimit)
	{
		if (getIsDeselect())
		{
			select(e.pos);
			_parent->selectCheck(this);
		}
		
		if (getIsSelect())
		{
			if (_eSelectDrapState == eDrap)
			{
				fitSelectPos();
			}
			_parent->selectCheck(this);
		}
		
	}
	else
	{
		if (getIsDeselect())
		{
			_centerVec = e.delta * ofGetFrameRate() * 0.5;
		}
		if (getIsSelect())
		{
			fitSelectPos();
		}
	}
}

//--------------------------------------
ofRectangle wallList::getInputArea()
{
	ofVec2f pos_((_animDrawPosX.getCurrentValue() - _animDrawWidth.getCurrentValue() * 0.5), 0);
	pos_.x += _parent->getWallRectPos().x;
	return ofRectangle(pos_, _animDrawWidth.getCurrentValue(), _baseArea.getHeight());
}
#pragma endregion


