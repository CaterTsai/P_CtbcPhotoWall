#pragma once

#include "ofMain.h"
#include "nameManager.h"
#include "ctbcEvent.h"

#pragma region Define
#define USE_MOUSE
#define PHOTO_TYPE unsigned char
#define PHOTO_ID_MASK 0x000fffff
#define PHOTO_TYPE_MASK 0xfff00000
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
const int cWindowHeight = 1080;

//Wall
//const int cMinimumPhotoWidth = 80;
//const int cMinimumPhotoHeight = 45;
const int cPhotoWallCategoryWidth = 480;
const int cMinimumPhotoWidth = (int)cPhotoWallCategoryWidth / 12.0;
const int cMinimumPhotoHeight = (int)cMinimumPhotoWidth * 9.0/16.0;
const int cDefaultPhotoListNum = (int)ceil(cWindowHeight / (float)cMinimumPhotoHeight);

const float cSelectAnimLength = 0.5f;
const int cSelectWidth = cPhotoWallCategoryWidth * 0.25f;

const int cCategoryNum = 4;
#pragma endregion
