#include "fontMgr.h"

//-----------------------------------------------------------------------------
void fontMgr::setup(string fontPath)
{
	ofxTrueTypeFontUC regularfont_, blackfont_;

	_isSetup = true;

	regularfont_.setGlobalDpi(72);
	_isSetup &= regularfont_.loadFont(fontPath + "regular.otf", 20);

	blackfont_.setGlobalDpi(72);
	_isSetup &= blackfont_.loadFont(fontPath + "black.otf", 20);

	if (!_isSetup)
	{
		ofLog(OF_LOG_ERROR, "[fontMgr::setup]load font failed");
	}

}

//-----------------------------------------------------------------------------
void fontMgr::drawString(eFontType type, string msg, ofVec2f pos)
{
	auto iter_ = _fontMap.find(type);
	if (iter_ != _fontMap.end())
	{
		iter_->second.drawString(msg, pos.x, pos.y);
	}
	
}

//-----------------------------------------------------------------------------
void fontMgr::setFontSize(eFontType type, int size)
{
	auto iter_ = _fontMap.find(type);
	if (iter_ != _fontMap.end())
	{
		if (iter_->second.getFontSize() != size)
		{
			iter_->second.reloadFont(size);
		}
	}
}

//-----------------------------------------------------------------------------
void fontMgr::setFontLetterSpace(eFontType type, float size)
{
	auto iter_ = _fontMap.find(type);
	if (iter_ != _fontMap.end())
	{
		iter_->second.setLetterSpacing(size);
	}
}

//-----------------------------------------------------------------------------
ofRectangle fontMgr::getStringBoundingBox(eFontType type, string msg)
{
	auto iter_ = _fontMap.find(type);
	if (iter_ != _fontMap.end())
	{
		return iter_->second.getStringBoundingBox(msg, 0, 0);
	}
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