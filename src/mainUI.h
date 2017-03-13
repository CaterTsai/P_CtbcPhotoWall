#pragma once

#include "constParameter.h"
#include "inputEventMgr.h"
#include "fontMgr.h"
#include "dataHolder.h"
#include "imageRender.h"
#include "ofxAnimatableFloat.h"

class wallMgr;
class mainUI : public inputEvent
{
#pragma region CLASS baseUnit
private:
	class baseUnit {
	public:
		baseUnit() {};
		baseUnit(ofColor c);

		void update(float delta);
		void draw(bool isBig, ofVec2f pos, int width, int height, bool displayZH = true);

		void setNameBig(ofImage & zhText, ofImage & enText);
		void setNameSmall(ofImage & zhText, ofImage & enText);

		void open();
		void close();
		bool isOpen();
		bool isClose();

		void directOpen();
		void directClose();
	private:
		ofRectangle getDrawRect(bool isBig, int width, int height, bool displayZH);

	private:
		enum eAnimState
		{
			eClose	=	0
			,eBPIn
			,eTextIn
			,eOpen
			,eTextOut
			,eBPOut
		}_eState;

		ofColor _color;
		ofImage _nameBigZH, _nameBigEN;
		ofImage _nameSmallZH, _nameSmallEN;
		ofxAnimatableFloat	_animTextWidth, _animBackplaneWidth;
	};
#pragma endregion
	
#pragma region Basic Method
public:
	mainUI();
	void setup(wallMgr* wallMgr, ofVec2f drawPos, ePhotoPrimaryCategory eCategory);
	void update(float delta);
	void draw(bool isZH);

	void start();
	void end();
	void toBigMenu();
	void toSmallMenu();
	bool isReady();
	void changeCategory(ePhotoPrimaryCategory eCategory);

private:
	bool setupUI();
	void resetUI();
	void createTextImg(string text, eFontType eType, ofImage& img);
	ofImage combineUIName(ofImage& name, ofImage& subtitle);
private:
	enum eMainUIState
	{
		eMainUIDisable = 0
		,eMainUIDisplayBig
		,eMainUIDisplaySmall
	}_eMainUIState;
	bool _setup;
	ofVec2f _centerPos;
	ePhotoPrimaryCategory	_category;
	map<ePhotoPrimaryCategory, baseUnit>	_mainUIMap;
	wallMgr* _parentWallMgr;
#pragma endregion

#pragma region Big Menu
//---------------------------------------
//Big Menu
private:
	void setupBigMenu();
	void updateBigMenu(float delta);
	void drawBigMenu(bool isZH);
	void drawMain(bool isZH);
	void drawMini(bool isZH);
	void drawBtn(bool isZH);

	void resetBigMenuAnim();
	void mainIn();
	void mainOut();
	void miniIn();
	void miniOut();

	void bigMenuInputCheck(ofVec2f pos);
	ofRectangle getBigMenuInputArea();

	void bigMenuAnimStateCheck();
	ePhotoPrimaryCategory getSelectMiniCategory(ofVec2f selectPos);

	string getBtnName(ePhotoPrimaryCategory category, bool isZH);
private:
	enum eBigUIState
	{
		eBigUIClose = 0
		, eBigUIMainIn
		, eBigUIMiniIn
		, eBigUIOpen
		, eBigUIMiniOut
		, eBigUIMainOut
	}_eBigUIState;

	ofVec2f _mainPos, _miniPos, _btnPos;

	ofxAnimatableFloat	_animMiniPosX[3];
	ofxAnimatableFloat	_animBtn;
#pragma endregion
	
#pragma region Small Menu
//---------------------------------------
//Small Menu
private:
	void setupSmallMenu();
	void updateSmallMenu(float delta);
	void drawSmallMenu(bool isZH);	
	
	void smallUIIn();
	void smallUIOut();
	void smallMenuAnimStateCheck();

	void smallMenuInputCheck(ofVec2f pos);
	ofRectangle getSmallMenuInputArea();
private:
	enum eSmallUIState
	{
		eSmallUIClose = 0
		,eSmallUIIn
		,eSmallOpen
		,eSmallUIOut
	}_eSmallUIState;

	ofVec2f _smallPos;
#pragma endregion
	
#pragma region Input
private:
	void enableInput();
	void disableInput();
	void inputRelease(inputEventArgs e) override;
	ofRectangle	getInputArea() override;
#pragma endregion

};