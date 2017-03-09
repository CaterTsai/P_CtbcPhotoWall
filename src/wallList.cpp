#include "wallList.h"
#include "wallMgr.h"

//--------------------------------------
wallList::wallList(wallMgr* parent, ePhotoPrimaryCategory eCategroy, ofRectangle drawArea, bool defaultSmile)
	:_baseArea(drawArea)
	,_eSelectState(eDeselect)
	, _eMoveCenterYState(eStable)
	,_centerUnitPos(0.0)
	, _eCategroy(eCategroy)
	, _parent(parent)
	, _needRemove(false)
	, _isChangeType(false)
	, _selectPhotoType(-1)
	, _isSmile(false)
{
	_defaultSmile = defaultSmile;
	setupAnimation(drawArea.getCenter().x, _baseArea.getWidth());
	initWallUnits((eCategroy == ePhotoCategory_4) && defaultSmile);
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
	checkMoveCenterYState();	
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
void wallList::reset()
{	
	if (_isChangeType)
	{
		changeCategory(_eCategroy);
	}
}

//--------------------------------------
float wallList::getBaseWidth()
{
	return _baseArea.getWidth();
}

//--------------------------------------
int wallList::getBasePosX()
{
	return _baseArea.getCenter().x;
}

//--------------------------------------
int wallList::getDrawPosX()
{
	return _animDrawPosX.getCurrentValue();
}

#pragma region Center
//--------------------------------------
void wallList::resetCenter()
{
	_centerUnitPos.set(0);
	_centerVec.set(0.0f, ofRandom(-1, 1)>0?ofRandom(cWallListMoveVecMin, cWallListMoveVecMax):ofRandom(-cWallListMoveVecMax, -cWallListMoveVecMin));
	_centerBaseVec = _centerVec;
}

//--------------------------------------
void wallList::updateCenter(float delta)
{
	if (_eMoveCenterYState == eStable)
	{
		if (abs(_centerVec.y) > abs(_centerBaseVec.y))
		{
			_centerVec.y *= 0.9;
		}

		float diff_ = _centerVec.y * delta;
		int height_ = (*_wallUnitList.begin())->getHeight();
		_centerUnitPos.y += diff_;
	}
	else
	{
		_centerUnitPos.y = _animDrawPosY.getCurrentValue();
	}

	fixCenterUnitPos();
}

//--------------------------------------
void wallList::updateSelectCenter(float delta)
{
	if (_eSelectState == eZoomIn || _eSelectState == eZoomOut)
	{
		_selectWallUnit.pos.y = _animSelectPosY.getCurrentValue();
		_centerUnitPos.y = getCenterUnitPosYFromSelect();
	}
	else
	{
		if (_eMoveCenterYState == eMove)
		{
			_centerUnitPos.y = _animDrawPosY.getCurrentValue();
		}
	}

	fixCenterUnitPos();
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
wallUnitInfo wallList::foundWallUnit(int index)
{
	wallUnitInfo _rVal;
	if (index >= 0 && _wallUnitList.size() > index)
	{
		int upDiff_ = 0;
		for (int idx_ = _wallUnitList.size() - 1; idx_ > index; idx_--)
		{
			upDiff_ += _wallUnitList[idx_]->getHeight();
		}

		if (_centerUnitPos.y - upDiff_ >= 0)
		{
			_rVal.id = index;
			_rVal.pos.set(_centerUnitPos.x, _centerUnitPos.y - (upDiff_ + _wallUnitList[index]->getHeight()));
		}
		else
		{
			_rVal.id = index;
			_rVal.pos.set(_centerUnitPos.x, _centerUnitPos.y + (_wallTotalHeight - upDiff_ - _wallUnitList[index]->getHeight()));
		}
	}


	return _rVal;
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

//--------------------------------------
void wallList::fixCenterUnitPosByUnit(int idx, int posY)
{
	if (idx < 0 || idx >= _wallUnitList.size())
	{
		ofLog(OF_LOG_ERROR, "[wallList::fixCenterUnitPosByUnit]out of range");
		return;
	}
	int dist_ = 0;
	for (int idx_ = 0; idx_ < idx; idx_++)
	{
		dist_ += _wallUnitList[idx_]->getHeight();
	}

	if (posY - (dist_ - _wallUnitList[0]->getHeight())> 0)
	{
		_centerUnitPos.y = posY - dist_ ;
	}
	else
	{
		_centerUnitPos.y = posY + (_wallTotalHeight - dist_);
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

		_wallUnitList[_selectWallUnit.id]->setClick(true);
		
		//trigger animation
		_eSelectState = eZoomIn;

		_animDrawPosX.animateTo(getAnimMoveX());		
		_animDrawWidth.animateTo(cSelectWidth);
		fixSelectPos();
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
		_animDrawPosX.animateTo(getBasePosX());
		_animDrawWidth.animateTo(_baseArea.getWidth());
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------
int wallList::getSelectPhotoID()
{
	if (getIsDeselect())
	{
		return -1;
	}
	else
	{
		auto selectUnit_ = dynamic_cast<photoUnit*>(_wallUnitList[_selectWallUnit.id].get());
		return selectUnit_->getPhotoHeader().id;
	}
}

//--------------------------------------
int wallList::getSelectTopPosY()
{
	int rVal_ = 0;

	if (getIsSelect())
	{
		rVal_ = _selectWallUnit.pos.y;
	}
	return rVal_;
}

//--------------------------------------
int wallList::getSelectBottomPosY()
{
	int rVal_ = 0;

	if (getIsSelect())
	{
		try
		{
			rVal_ = _selectWallUnit.pos.y + _wallUnitList.at(_selectWallUnit.id)->getHeight();
		}
		catch (const std::exception&)
		{
			ofLog(OF_LOG_ERROR, "[wallList::getSelectBottomPosY]out of range");
		}
		
	}
	return rVal_;
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

	_animDrawPosY.setDuration(0.2);
	_animDrawPosY.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animDrawPosY.setCurve(AnimCurve::BOUNCY);

	_animSelectPosY.setDuration(cSelectAnimLength);
	_animSelectPosY.setRepeatType(AnimRepeat::PLAY_ONCE);
	_animSelectPosY.reset(width);
}

//--------------------------------------
void wallList::updateAnimation(float delta)
{
	_animDrawWidth.update(delta);
	_animDrawPosX.update(delta);
	_animDrawPosY.update(delta);
	_animSelectPosY.update(delta);
}

//--------------------------------------
void wallList::movePosY(int posY, float t)
{
	_animDrawPosY.reset(_centerUnitPos.y);
	_animDrawPosY.setDuration(t);
	_animDrawPosY.animateTo(posY);
	_eMoveCenterYState = eMove;
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
				_parent->textUIin();
				_parent->scrollUIin();
				_parent->closeUIin();
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
				_centerVec.set(0.0f, ofRandom(-1, 1)>0 ? ofRandom(cWallListMoveVecMin, cWallListMoveVecMax) : ofRandom(-cWallListMoveVecMax, -cWallListMoveVecMin));
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
void wallList::checkMoveCenterYState()
{
	//TODO
	if (_eMoveCenterYState == eMove && _animDrawPosY.hasFinishedAnimating() && _animDrawPosY.getPercentDone() == 1.0)
	{
		_eMoveCenterYState = eStable;

		if (getIsSelect())
		{
			_wallUnitList[_selectWallUnit.id]->setClick(true);
			_parent->setTextUIVisible(true);
			_parent->setScrollUIVisible(true);
			_parent->setCloseUIVisible(true);
	
		}

		if (_needRemove)
		{
			removeWallUnitCheck();
			_needRemove = false;
		}
	}
}

//--------------------------------------
void wallList::removeWallUnitCheck()
{
	//TODO
	int fixID_, fixPosY_;
	int insertNum_ = _insertEnd - _insertStart + 1;

	if (_wallUnitList.size() - insertNum_ >= cDefaultPhotoListNum)
	{
		removeWallUnits(0, _insertStart);
		removeWallUnits(insertNum_, _wallUnitList.size());

		if (getIsSelect())
		{
			_selectWallUnit.id -= _insertStart;
			fixID_ = _selectWallUnit.id;
			fixPosY_ = _selectWallUnit.pos.y;
		}
		else if (getIsDeselect())
		{
			int id_ = (insertNum_) * 0.5;
			fixID_ = id_;
			fixPosY_ = _baseArea.getHeight() * 0.5;
		}

		
		fixCenterUnitPosByUnit(fixID_, fixPosY_);
	}
}

//--------------------------------------
int wallList::getAnimMoveX()
{
	float halfSelectWidth_ = cSelectWidth * 0.5f;
	int moveX_ = getBasePosX();
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
void wallList::fixSelectPos()
{
	int selectHeight_ = _wallUnitList[_selectWallUnit.id]->getHeight(cSelectWidth);
	int selectMinPosY_ = _selectWallUnit.pos.y;
	int selectMaxPosY_ = _selectWallUnit.pos.y + selectHeight_ + cTextUIHeight;
	
	_animSelectPosY.reset(_selectWallUnit.pos.y);
	if (selectMinPosY_ < 0)
	{
		_animSelectPosY.animateTo(0);
	}
	else if(selectMaxPosY_ > cWindowHeight)
	{
		_animSelectPosY.animateTo(_selectWallUnit.pos.y + (cWindowHeight - selectMaxPosY_));
	}
}

//--------------------------------------
void wallList::attractDrawPos()
{
	auto nearestWallUnit_ = foundWallUnit(_selectWallUnit.pos, true);
	_selectWallUnit.id = nearestWallUnit_.id;
	float diffY_ = nearestWallUnit_.pos.y - _selectWallUnit.pos.y;

	movePosY(_centerUnitPos.y - diffY_, 0.3);
	_parent->updateTextUI(getSelectPhotoID());
}

//--------------------------------------
void wallList::moveSelectPos(wallUnitInfo newSelectUnit)
{
	_selectWallUnit.id = newSelectUnit.id;
	float diffY_ = _wallTotalHeight - abs(newSelectUnit.pos.y - _selectWallUnit.pos.y);
	movePosY(_centerUnitPos.y + diffY_, cMoveWallListPosYLength);

	_parent->updateTextUI(getSelectPhotoID());
}

#pragma endregion

#pragma region WallUnit
//--------------------------------------
void wallList::selectType(PHOTO_TYPE type)
{
	wallUnitInfo start, end;
	findDisplayRange(start, end);
	int insertNum_ = insertWallUnits(start.id, type);
	_insertID = start.id + insertNum_ - cMaxSelectPhotoDisplayNum;
	if (_insertID < 0)
	{
		_insertID += _wallUnitList.size();
	}
	auto insert_ = foundWallUnit(_insertID);
	
	if (_eCategroy == ePhotoCategory_4 && type == dataHolder::GetInstance()->getSmileType())
	{
		registerSmilePhoto();
	}
	else
	{
		unregisterSmilePhoto();
	}

	_isChangeType = true;
	_selectPhotoType = type;
	_needRemove = true;
	_insertStart = start.id;
	_insertEnd = start.id + insertNum_ - 1;
	
	moveSelectPos(insert_);
}

//--------------------------------------
void wallList::changeCategory(ePhotoPrimaryCategory category)
{
	_eCategroy = category;
	wallUnitInfo start, end;
	findDisplayRange(start, end);

	int insertNum_ = 0;
	if ((_eCategroy == ePhotoCategory_4) && _defaultSmile)
	{
		registerSmilePhoto();
		insertNum_ = insertWallUnits(start.id, dataHolder::GetInstance()->getSmileType());
	}
	else
	{
		unregisterSmilePhoto();
		insertNum_ = insertWallUnits(start.id);
	}
	auto insert_ = foundWallUnit(start.id + (insertNum_ * 0.5));

	_needRemove = true;
	_isChangeType = false;
	_selectPhotoType = -1;
	_insertStart = start.id;
	_insertEnd = start.id + insertNum_ - 1;
	float diffY_ = _wallTotalHeight - abs(insert_.pos.y - _baseArea.getHeight() * 0.5);
	movePosY(_centerUnitPos.y + diffY_, cMoveWallListPosYLength);
}

//--------------------------------------
PHOTO_TYPE wallList::getSelectType()
{
	return _selectPhotoType;
}

//--------------------------------------
bool wallList::isChangeType()
{
	return _isChangeType;
}

//--------------------------------------
bool wallList::getCanSelectType()
{
	return _eSelectState == eSelect && _eMoveCenterYState == eStable;
}

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
void wallList::addWallUnit(int index, ofPtr<wallUnit> newUnil)
{
	_wallUnitList.insert(_wallUnitList.begin() + index, newUnil);
}

//--------------------------------------
void wallList::initWallUnits(bool isSmile)
{
	vector<int> photoIDList_;

	if (isSmile)
	{
		photoIDList_ = dataHolder::GetInstance()->getPhotoID(
			_eCategroy, 
			dataHolder::GetInstance()->getSmileType()
			);
		registerSmilePhoto();
	}
	else
	{
		photoIDList_ = dataHolder::GetInstance()->getPhotoID(_eCategroy);
	}

	try
	{
		random_shuffle(photoIDList_.begin(), photoIDList_.end());

		int idx_ = 0;
		for (auto& iter_ : photoIDList_)
		{
			if (idx_ > cDefaultPhotoListNum)
			{
				break;
			}

			auto photoHeader_ = dataHolder::GetInstance()->getPhotoHeader(iter_);
			addWallUnit(ofPtr<wallUnit>(new photoUnit(photoHeader_, _animDrawWidth.getCurrentValue())));

			idx_++;

		}
		updateWallTotalHeight();
	}
	catch (const std::exception&)
	{
		ofLog(OF_LOG_ERROR, "[initWallUnits]Get photoIDList exception");
	}

	
}

//--------------------------------------
int wallList::insertWallUnits(int index, PHOTO_TYPE type)
{
	auto photoIDList_ = dataHolder::GetInstance()->getPhotoID(_eCategroy, type);

	random_shuffle(photoIDList_.begin(), photoIDList_.end());

	int idx_ = 0;
	for (auto& iter_ : photoIDList_)
	{
		if (idx_ >= cMaximumInsertPhotoNum)
		{
			break;
		}
		auto photoHeader_ = dataHolder::GetInstance()->getPhotoHeader(iter_);
		addWallUnit(index, ofPtr<wallUnit>(new photoUnit(photoHeader_, _animDrawWidth.getCurrentValue())));
		idx_++;

	}
	updateWallTotalHeight();

	return idx_;
}

//--------------------------------------
int wallList::insertWallUnits(int index)
{
	auto photoIDList_ = dataHolder::GetInstance()->getPhotoID(_eCategroy);

	random_shuffle(photoIDList_.begin(), photoIDList_.end());

	int idx_ = 0;
	for (auto& iter_ : photoIDList_)
	{
		if (idx_ > cDefaultPhotoListNum)
		{
			break;
		}

		auto photoHeader_ = dataHolder::GetInstance()->getPhotoHeader(iter_);
		addWallUnit(index, ofPtr<wallUnit>(new photoUnit(photoHeader_, _animDrawWidth.getCurrentValue())));

		idx_++;

	}
	updateWallTotalHeight();
	return cDefaultPhotoListNum;
}

//--------------------------------------
void wallList::insertWallUnit(int index, stPhotoHeader newPhotoHeader)
{
	addWallUnit(index, ofPtr<wallUnit>(new photoUnit(newPhotoHeader, _animDrawWidth.getCurrentValue())));
	updateWallTotalHeight();
}

//--------------------------------------
void wallList::removeWallUnits(int start, int end)
{
	if (start < 0 || _wallUnitList.size() < end)
	{
		ofLog(OF_LOG_ERROR, "[wallList::removeWallUnits]Wrong index");
		return;
	}
	
	_wallUnitList.erase(_wallUnitList.begin() + start, _wallUnitList.begin() + end);
	updateWallTotalHeight();
}

//--------------------------------------
void wallList::clearWallUnits()
{
	_wallUnitList.clear();
	updateWallTotalHeight();
}

//--------------------------------------
void wallList::findDisplayRange(wallUnitInfo& start, wallUnitInfo& end)
{
	float diffToTop_ = _centerUnitPos.y - 0;
	float diffToDown_ = _baseArea.getHeight() - _centerUnitPos.y;

	start = foundUnitUp(abs(diffToTop_));
	end = (diffToDown_ > 0 ? foundUnitDown(diffToDown_) : foundUnitUp(abs(diffToDown_)));

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

#pragma region Smile
//--------------------------------------
void wallList::onNewSmilePhoto(stPhotoHeader & smilePhoto)
{	
	wallUnitInfo start, end;
	findDisplayRange(start, end);
	insertWallUnit(start.id, smilePhoto);
	
	if (getIsDeselect())
	{
		_needRemove = false;
		auto insert_ = foundWallUnit(start.id);		
		float diffY_ = _wallTotalHeight - abs(insert_.pos.y - _baseArea.getHeight() * 0.5);
		movePosY(_centerUnitPos.y + diffY_, cMoveWallListPosYLength);
	}	
}

//--------------------------------------
void wallList::registerSmilePhoto()
{
	if (!_isSmile)
	{
		ofAddListener(dataHolder::GetInstance()->_onNewSmilePhoto, this, &wallList::onNewSmilePhoto);
		_isSmile = true;
	}
	
}

//--------------------------------------
void wallList::unregisterSmilePhoto()
{
	if (_isSmile)
	{
		ofRemoveListener(dataHolder::GetInstance()->_onNewSmilePhoto, this, &wallList::onNewSmilePhoto);
		_isSmile = false;
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
}

//--------------------------------------
void wallList::inputDrag(inputEventArgs e)
{
	if (_eSelectState != eZoomIn && _eSelectState != eZoomOut && _eMoveCenterYState != eMove)
	{	

		_centerUnitPos.y += e.delta.y;

		if (getIsSelect() && abs(e.diffPos.y) > cInputTriggerDiffLimit && _wallUnitList[_selectWallUnit.id]->getClick())
		{
			_parent->setTextUIVisible(false);
			_parent->setScrollUIVisible(false);
			_parent->setCloseUIVisible(false);
			_wallUnitList[_selectWallUnit.id]->setClick(false);
		}
	}
}

//--------------------------------------
void wallList::inputRelease(inputEventArgs e)
{
	if (e.holdTime <= cInputHoldLimit && abs(e.diffPos.y) < cInputTriggerDiffLimit)
	{
		if (getIsDeselect() && _parent->isCanSelect())
		{
			select(e.pos);
			_parent->selectCheck(this);
		}
		else
		{
			_centerVec = _centerBaseVec;
		}
	}
	else
	{
		if (getIsDeselect())
		{
			_centerVec = e.delta * ofGetFrameRate() * 0.5;
			
		}
		if (getIsSelect() && _eMoveCenterYState == eStable)
		{
			attractDrawPos();
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
