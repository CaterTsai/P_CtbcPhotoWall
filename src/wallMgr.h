#pragma once

#include "wallList.h"
#include "mainUI.h"
#include "textUI.h"
#include "scrollUI.h"

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

	void addWallList(int width);

	ofVec2f getWallRectPos();
	int getWallRectWidth();

private:
	void setupCheck();
	int getListTotalWidth();

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
	ePhotoPrimaryCategory	_eCategory;
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
private:
	
	void drawTextUI();
private:
	bool _isTextUIVisible;
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

#pragma region Select
public:
	void selectCheck(wallList* selectList);
private:
	wallList* _selectWallList;
#pragma endregion

#pragma region Input
private:
	void enableInput();
	void disableInput();
	void inputRelease(inputEventArgs e) override;
	ofRectangle	getInputArea() override;
#pragma endregion
	
};