#pragma once

#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"

enum eFontType
{
	eFontRegular = 0
	,eFontBlack
};

class fontMgr
{
public:
	void setup(string fontPath);
	
	
	void drawString(eFontType type, string msg, ofVec2f pos);
	void setFontSize(eFontType type, int size);
	void setFontLetterSpace(eFontType type, float spaceSize);
	ofRectangle getStringBoundingBox(eFontType type, string msg);
private:
	bool _isSetup;
	map<eFontType, ofxTrueTypeFontUC>	_fontMap;

public:
	static string ws2s(const wstring& wstr);

#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	fontMgr();
	~fontMgr(){};
	void operator=(fontMgr const&) {};

public:
	static fontMgr* GetInstance();
	static void Destroy();

private:
	static fontMgr *_pInstance;
#pragma endregion
};