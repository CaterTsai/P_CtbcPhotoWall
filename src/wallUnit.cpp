#include "wallUnit.h"

#pragma region photo
//--------------------------------------
photoUnit::photoUnit(stPhotoHeader & photoIdx, int width)
	:_photoIndex(photoIdx)
	, wallUnit::wallUnit(ePhotoUnit)
{
	setRatio();
	_width = width;
	_height = static_cast<int>(width * _photoRatio + 0.5f);
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
void photoUnit::draw(ofVec2f pos, bool isSelectList)
{
	ofPushStyle();
	ofSetColor(255);
	ofVec2f drawPos_(pos);
	drawPos_.x += (cPhotoUnitInterval) * 0.5;
	drawPos_.y += (cPhotoUnitInterval) * 0.5;
	photoRender::GetInstance()->drawThumb(_photoIndex, drawPos_, _width - cPhotoUnitInterval, _height - cPhotoUnitInterval);
	
	if (isSelectList && !_isClick)
	{
		ofSetColor(cSelectCoverColor, cSelectCoverAlpha);
		ofFill();
		ofRect(drawPos_, _width - cPhotoUnitInterval, _height - cPhotoUnitInterval);
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


