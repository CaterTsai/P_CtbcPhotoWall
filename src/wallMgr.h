#pragma once

#include "wallList.h"
#include "mainUI.h"
#include "textUI.h"
#include "scrollUI.h"
#include "closeUI.h"

class wallMgr : public inputEvent
{
public:
	wallMgr();
	~wallMgr();

#pragma region Base Method
	void setup(ePhotoPrimaryCategory category, ofRectangle wallRect);
	void update(float delta);
	void draw();
	void drawSelect();
	void drawShadow();

	void start();
	void end();

	void addWallList(int width, bool canBeSmile = false);

	ofVec2f getWallRectPos();
	int getWallRectWidth();

private:
	void setupCheck();
	int getListTotalWidth();
	void resetWallList();

	void enableWallListInput();
	void disableWallListInput();

private:
	enum eWallState
	{
		eWallIdle = 0
		, eWallMainUI
		, eWallPhoto
	}_eWallState;
	bool	_isSetup;
	ofRectangle	_wallRect;
	ePhotoPrimaryCategory	_eCategory, _backupCategory;
	vector<ofPtr<wallList>> _wallListMgr;
#pragma endregion

#pragma region mainUI
public:
	void mainUIin();
	void mainUIout();

	void changeLanguage();
	void changeCategory(ePhotoPrimaryCategory eCategory);
private:
	bool _isDisplayZH;
	mainUI	_mainUI;
#pragma endregion
	
#pragma region textUI
public:
	void textUIin();
	void textUIout();

	void setTextUIVisible(bool val);
	void updateTextUI(int photoID);

	ofVec2f getTextUIPos();
public:
	void checkUpdateText();
	void onTextIsLoad(int &pid);

private:
	
	void drawTextUI();
private:
	bool _isTextUIVisible, _needUpdateText;
	
	textUI	_textUI;
#pragma endregion

#pragma region scrollUI
public:
	void scrollUIin();
	void scrollUIout();

	void setScrollUIVisible(bool val);
	void updateScrollUI();

	ofVec2f getScrollUIPos();

	void selectType(PHOTO_TYPE type);
private:

	void drawScrollUI();
private:
	bool _isScrollUIVisible;
	scrollUI	_scrollUI;
#pragma endregion

#pragma region closeUI
public:
	void closeUIin();
	void closeUIout();
	void setCloseUIVisible(bool val);

	void closeSelect();
	ofVec2f getCloseUIPos();
	
private:
	void drawCloseUI();

private:
	bool _isCloseUIVidible;
	closeUI _closeUI;

#pragma endregion

#pragma region Select
public:
	void canSelect();
	bool isCanSelect();
	void selectCheck(wallList* selectList);
private:
	bool _canSelect;
	wallList* _selectWallList;
#pragma endregion

#pragma region Input
public:
	void enableInput();
	void disableInput();
private:
	void inputRelease(inputEventArgs e) override;
	ofRectangle	getInputArea() override;
#pragma endregion
	
};