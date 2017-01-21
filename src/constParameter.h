#pragma once

#include "ofMain.h"
#include "nameManager.h"
#include "ctbcEvent.h"

#pragma region Define
#define USE_MOUSE
#define PHOTO_TYPE unsigned short
#define PHOTO_ID_MASK 0x0000ffff
#define PHOTO_HEADER_MASK 0xffff0000
#define PHOTO_SHAPE_MASK 0x000f0000
#define PHOTO_TYPE_MASK 0x0ff00000
#define PHOTO_CATEGORY_MASK 0xf0000000
#pragma endregion


#pragma region ENUM
enum eWallUnitType : int
{
	ePhotoUnit = 0
	,eMenuUnit
};
enum ePhotoPrimaryCategory : unsigned char
{
	ePhotoCategory_1 = 0
	,ePhotoCategory_2
	,ePhotoCategory_3
	,ePhotoCategory_4
};

enum ePhotoShape : int
{
	ePhotoNormalHorizontal = 0	// 4:3
	,ePhotoNormalVertical		// 3:4
	,ePhotoWideHorizontal		// 16:9
	,ePhotoWideVertical			// 9:16

};

enum eInputSystemLevel : int
{
	eInputTop = 0
	,eInputMainUI
	,eInputWallUI
	,eInputWallMgr
	,eInputWallSelectList
	,eInputWallList
};

#pragma endregion

#pragma region Struct
struct stPhotoHeader
{
	int id;
	PHOTO_TYPE type;
	ePhotoPrimaryCategory category;
	ePhotoShape shape;
	string thumbnailPath;
};

struct stPhotoData
{
	stPhotoHeader header;
	string title, msg;
	string sourcePath;
	
};
#pragma endregion

#pragma region const parameter
const int cWindowWidth = 1920;
const int cWindowHeight = cWindowWidth * 0.421875;
const int cPhotoWallCategoryWidth = cWindowWidth * 0.25;

//input event manager
const int cInputEventLevel = 10;
const float cInputHoldLimit = 0.5f;
const float cInputTriggerDiffLimit = cPhotoWallCategoryWidth * 0.05f;

//Wall
const float cWallListMoveVecRatio = 5.0f;
const float cWallListMoveVecMax = 80.0f;
const int cMinimumPhotoWidth = (int)cPhotoWallCategoryWidth / 10.0;
const int cMinimumPhotoHeight = (int)cMinimumPhotoWidth * 9.0/16.0;
const int cPhotoUnitInterval = 5;
const int cDefaultPhotoListNum = (int)ceil(cWindowHeight / (float)cMinimumPhotoHeight);
const ofColor cSelectCoverColor(0, 167, 157);
const float cSelectCoverAlpha = 255 * 0.8;

const float cSelectAnimLength = 0.5f;
const int cSelectWidth = cPhotoWallCategoryWidth * 0.4f;
const int cSelectShdowWidth = cSelectWidth * 3.0;
const int cCategoryNum = 1;

//UI
const int cMainUIWidth = cPhotoWallCategoryWidth * 0.74;
const int cMainUIHeight = cMainUIWidth * 0.4;
const int cMainUIAlpha = 255 * 0.8;

const int cMainUIUnitWidth = cMainUIWidth * 0.85;
const int cMainUIUnitHeight = cMainUIHeight;
const int cMainUIUnitMinWidth = cMainUIWidth * 0.15;
const int cMainUIUnitMinHeight = cMainUIHeight * 0.334;
const int cMainUIBtnWidth = cMainUIWidth * 0.15;
const int cMainUIBtnHeight = cMainUIBtnWidth;
const int cMainUIFontSize = cMainUIHeight * 0.22;
const int cMainUIFontENSize = cMainUIHeight * 0.1;


#pragma endregion
