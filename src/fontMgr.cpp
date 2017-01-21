#include "fontMgr.h"

//-----------------------------------------------------------------------------
void fontMgr::setup(string fontPath)
{
	_isSetup = true;

	_fontList[eFontMainUIZH].setGlobalDpi(72);
	_fontList[eFontMainUIZH].setLetterSpacing(1.2f);
	_isSetup &= _fontList[eFontMainUIZH].loadFont(fontPath + "black.otf", cMainUIFontSize);

	_fontList[eFontMainUIEN].setGlobalDpi(72);
	_fontList[eFontMainUIEN].setSpaceSize(0.5);
	_isSetup &= _fontList[eFontMainUIEN].loadFont(fontPath + "english.TTF", cMainUIFontENSize);

	_fontList[eFontTextUIZH].setGlobalDpi(72);
	_isSetup &= _fontList[eFontTextUIZH].loadFont(fontPath + "regular.otf", 20);

	_fontList[eFontTextUIEN].setGlobalDpi(72);
	_isSetup &= _fontList[eFontTextUIEN].loadFont(fontPath + "regular.otf", 20);

	_fontList[eFontMenuUIZH].setGlobalDpi(72);
	_isSetup &= _fontList[eFontMenuUIZH].loadFont(fontPath + "regular.otf", 20);

	_fontList[eFontMenuUIEN].setGlobalDpi(72);
	_isSetup &= _fontList[eFontMenuUIEN].loadFont(fontPath + "regular.otf", 20);
	if (!_isSetup)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::setup]load font failed");
	}

}

//-----------------------------------------------------------------------------
void fontMgr::drawString(eFontType type, string msg, ofVec2f pos)
{
	if (!_isSetup)
	{
		return;
	}

	try
	{
		_fontList.at(type).drawString(msg, pos.x, pos.y);
	}
	catch (const std::exception& e)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::drawString]failed");
	}	
}


//-----------------------------------------------------------------------------
void fontMgr::setFontLetterSpace(eFontType type, float size)
{
	if (!_isSetup)
	{
		return;
	}

	try
	{
		_fontList.at(type).setLetterSpacing(size);
	}
	catch (const std::exception& e)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::setFontLetterSpace]failed");
	}
}

//-----------------------------------------------------------------------------
ofRectangle fontMgr::getStringBoundingBox(eFontType type, string msg)
{
	if (!_isSetup)
	{
		return ofRectangle();
	}

	ofRectangle stringBoundingBox_;
	try
	{
		stringBoundingBox_ = _fontList.at(type).getStringBoundingBox(msg, 0, 0);
	}
	catch (const std::exception& e)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::setFontLetterSpace]failed");
	}
	return stringBoundingBox_;
}

//-----------------------------------------------------------------------------
string fontMgr::ws2s(const wstring & wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, 0);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, 0);
	return strTo;
}

#pragma region Singleton
//--------------------------------------------------------------
fontMgr::fontMgr()
{}

//--------------------------------------------------------------
fontMgr* fontMgr::_pInstance = nullptr;
fontMgr * fontMgr::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new fontMgr();
	}
	return _pInstance;
}

//--------------------------------------------------------------
void fontMgr::Destroy()
{
	if (_pInstance == nullptr)
	{
		delete _pInstance;
	}
}
#pragma endregion