#pragma once

#include "constParameter.h"
#include "ofxTrueTypeFontUC.h"

enum eFontType : int
{
	eFontMainUI = 0
	,eFontMainUISubtitle
	,eFontMainUISmall
	,eFontTextUI
	,eFontTextUIContext
	,eFontMenuUITitle
	,eFontMenuUIContext
	,eFontNum
};

class fontMgr
{
public:
	
	void setup(string fontPath);	
	
	void drawString(eFontType type, string msg, ofVec2f pos, bool isZH);
	void setFontLetterSpace(eFontType type, float spaceSize, bool isZH);
	ofRectangle getStringBoundingBox(eFontType type, string msg, bool isZH);
	vector<int> getEachWordWidth(eFontType type, string msg, bool isZH);
private:
	bool _isSetup;
	array<ofxTrueTypeFontUC, eFontNum>	_fontList;
	array<ofTrueTypeFont, eFontNum> _fontENList;

public:
	static string ws2s(const wstring& wstr);
	static wstring s2ws(const string& str);

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