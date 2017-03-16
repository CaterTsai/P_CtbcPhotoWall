#include "fontMgr.h"

//-----------------------------------------------------------------------------
void fontMgr::setup(string fontPath)
{
	_isSetup = true;
	//------------------------------------------------
	//MainUI
	_fontList[eFontMainUI].setGlobalDpi(72);
	_fontList[eFontMainUI].setLetterSpacing(1.2f);
	_isSetup &= _fontList[eFontMainUI].loadFont(fontPath + "black.otf", cMainUIFontSize);

	_fontENList[eFontMainUI].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontMainUI].loadFont(fontPath + "en_black.ttf", cMainUIFontENSize);

	_fontList[eFontMainUISubtitle].setGlobalDpi(72);
	_isSetup &= _fontList[eFontMainUISubtitle].loadFont(fontPath + "meidium.otf", cMainUIFontSubtitleSize);

	_fontENList[eFontMainUISubtitle].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontMainUISubtitle].loadFont(fontPath + "en_meidium.ttf", cMainUIFontSubtitleENSize);

	_fontList[eFontMainUISmall].setGlobalDpi(72);
	_isSetup &= _fontList[eFontMainUISmall].loadFont(fontPath + "meidium.otf", cMainUIFontSmallSize);

	_fontENList[eFontMainUISmall].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontMainUISmall].loadFont(fontPath + "en_meidium.ttf", cMainUIFontSmallENSize);

	//--------------------------------------------------

	_fontList[eFontMainUI].setGlobalDpi(72);
	_fontList[eFontMainUI].setLetterSpacing(1.2f);
	_isSetup &= _fontList[eFontMainUI].loadFont(fontPath + "black.otf", cMainUIFontSize);

	_fontENList[eFontMainUI].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontMainUI].loadFont(fontPath + "en_meidium.ttf", cMainUIFontENSize);


	_fontList[eFontTextUI].setGlobalDpi(72);
	_isSetup &= _fontList[eFontTextUI].loadFont(fontPath + "regular.otf", cTextUIFontSize);

	_fontENList[eFontTextUI].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontTextUI].loadFont(fontPath + "en_meidium.ttf", cTextUIFontSize);

	_fontList[eFontTextUIContext].setGlobalDpi(72);
	_isSetup &= _fontList[eFontTextUIContext].loadFont(fontPath + "regular.otf", cTextUIContextFontSize);

	_fontENList[eFontTextUIContext].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontTextUIContext].loadFont(fontPath + "en_meidium.ttf", cTextUIContextFontSize);

	_fontList[eFontMenuUITitle].setGlobalDpi(72);
	_isSetup &= _fontList[eFontMenuUITitle].loadFont(fontPath + "meidium.otf", cScrollUITitleFontSize);

	_fontENList[eFontMenuUITitle].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontMenuUITitle].loadFont(fontPath + "en_meidium.ttf", cScrollUITitleFontENSize);

	_fontList[eFontMenuUIContext].setGlobalDpi(72);	
	_isSetup &= _fontList[eFontMenuUIContext].loadFont(fontPath + "meidium.otf", cScrollUIContextFontSize);

	_fontENList[eFontMenuUIContext].setGlobalDpi(72);
	_isSetup &= _fontENList[eFontMenuUIContext].loadFont(fontPath + "en_meidium.ttf", cScrollUIContextFontENSize);
	if (!_isSetup)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::setup]load font failed");
	}

}

//-----------------------------------------------------------------------------
void fontMgr::drawString(eFontType type, string msg, ofVec2f pos, bool isZH)
{
	if (!_isSetup)
	{
		return;
	}

	try
	{
		if (isZH)
		{
			_fontList.at(type).drawString(msg, pos.x, pos.y);
		}
		else
		{
			_fontENList.at(type).drawString(msg, pos.x, pos.y);
		}
		
	}
	catch (const std::exception& e)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::drawString]failed");
		ofLog(OF_LOG_ERROR, e.what());
	}	
}


//-----------------------------------------------------------------------------
void fontMgr::setFontLetterSpace(eFontType type, float size, bool isZH)
{
	if (!_isSetup)
	{
		return;
	}

	try
	{
		if (isZH)
		{
			_fontList.at(type).setLetterSpacing(size);
		}
		else
		{
			_fontENList.at(type).setLetterSpacing(size);
		}
		
	}
	catch (const std::exception& e)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::setFontLetterSpace]failed");
	}
}

//-----------------------------------------------------------------------------
ofRectangle fontMgr::getStringBoundingBox(eFontType type, string msg, bool isZH)
{
	if (!_isSetup)
	{
		return ofRectangle();
	}

	ofRectangle stringBoundingBox_;
	try
	{
		if (isZH)
		{
			stringBoundingBox_ = _fontList.at(type).getStringBoundingBox(msg, 0, 0);
		}
		else
		{
			stringBoundingBox_ = _fontENList.at(type).getStringBoundingBox(msg, 0, 0);
		}
		
	}
	catch (const std::exception& e)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::getStringBoundingBox]failed");
		ofLog(OF_LOG_ERROR, e.what());
	}
	return stringBoundingBox_;
}

//-----------------------------------------------------------------------------
vector<int> fontMgr::getEachWordWidth(eFontType type, string msg, bool isZH)
{
	vector<int> eachWordWidth_;
	try
	{
		if (isZH)
		{
			_fontList.at(type).getEachWordWidth(msg, eachWordWidth_);
		}
		else
		{
			_fontENList.at(type).getEachWordWidth(msg, eachWordWidth_);
		}
		
	}
	catch (const std::exception& e)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::getEachWordWidth]failed");
		ofLog(OF_LOG_ERROR, e.what());
	}
	return eachWordWidth_;
}

//-----------------------------------------------------------------------------
string fontMgr::ws2s(const wstring & wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, 0);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, 0);
	return strTo;
}

//--------------------------------------------------------------
wstring fontMgr::s2ws(const string & str)
{
	int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
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