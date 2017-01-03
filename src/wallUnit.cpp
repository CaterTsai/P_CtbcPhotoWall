#include "wallUnit.h"

#pragma region photo
//--------------------------------------
photoUnit::photoUnit(stPhotoHeader & photoIdx, int width)
	:_photoIndex(photoIdx)
	, wallUnit::wallUnit(ePhotoUnit)
{
	setRatio();
	_width = width;
	_height = width * _photoRatio;
}

//--------------------------------------
void photoUnit::update(float delta, int width)
{	
	if (_width != width)
	{
		_width = width;
		_height = static_cast<int>(width * _photoRatio + 0.5f);
	}
}

//--------------------------------------
void photoUnit::draw(ofVec2f pos)
{
	ofPushStyle();
	ofSetColor(255);
	renderMgr::GetInstance()->drawThumb(_photoIndex, pos, _width, _height);

	if (_isClick)
	{
		ofSetColor(255, 0, 0, 200);
		ofFill();
		ofRect(pos, _width, _height);
	}
	ofPopStyle();
}

//--------------------------------------
void photoUnit::onclick(ofVec2f pos)
{
	_isClick = !_isClick;
}

//--------------------------------------
int photoUnit::getHeight(int width)
{
	return static_cast<int>(width * _photoRatio + 0.5f);;
}

//--------------------------------------
stPhotoHeader& photoUnit::getPhotoHeader()
{
	return _photoIndex;
}

//--------------------------------------
void photoUnit::setRatio()
{
	switch (_photoIndex.shape)
	{
		case ePhotoNormalHorizontal:
		{
			_photoRatio = 3.0f / 4.0f;
			break;
		}
		case ePhotoNormalVertical:
		{
			_photoRatio = 4.0f / 3.0f;
			break;
		}
		case ePhotoWideHorizontal:
		{
			_photoRatio = 9.0f / 16.0f;
			break;
		}
		case ePhotoWideVertical:
		{
			_photoRatio = 16.0f / 9.0f;
			break;
		}
	}
}
#pragma endregion


