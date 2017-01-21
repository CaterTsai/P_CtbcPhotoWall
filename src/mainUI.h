#pragma once

#include "constParameter.h"
#include "inputEventMgr.h"
#include "fontMgr.h"
#include "imageRender.h"
#include "ofxAnimatableFloat.h"
#include "ofxXmlSettings.h"

class wallMgr;
class mainUI : public inputEvent
{
#pragma region CLASS baseUnit
private:
	class baseUnit {
	public:
		baseUnit() {};
		baseUnit(ofImage& zhText, ofRectangle& zhRect, ofImage& enText, ofRectangle& enRect, ofColor c);

		void update(float delta);
		void draw(ofVec2f pos, int width, int height, bool displayZH = true);

		void open();
		void close();
		bool isOpen();
		bool isClose();

		void directOpen();
		void directClose();
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
		ofImage _textZH, _textEN;
		ofRectangle	_textRectZH, _textRectEN;
		ofxAnimatableFloat	_animTextWidth, _animBackplaneWidth;
	};
#pragma endregion
	
public:
	mainUI();
	void setup(wallMgr* wallMgr, ofVec2f drawPos, string xmlPath, ePhotoPrimaryCategory eCategory);
	void update(float delta);
	void draw(bool isZH);

	void open();
	void close();

private:
	void drawMain(bool isZH);
	void drawMini(bool isZH);
	void drawBtn(bool isZH);

	void setupMiniAnim();
	void miniIn();
	void miniOut();

	void animStateCheck();
	bool loadXml(string xmlPath);
	void createTextImg(string text, ofImage& img, ofRectangle& textRect);
	void createTextImgEN(string text, ofImage& img, ofRectangle& textRect);

	static ofColor getBPColor(ePhotoPrimaryCategory category);
	static string getBtnName(ePhotoPrimaryCategory category, bool isZH);
private:
	enum eUIState
	{
		eUIClose = 0
		,eUIMainIn
		,eUIMiniIn
		,eUIOpen
		,eUIMiniOut
		,eUIMainOut
	}_eUIState;
	bool _setup;
	ofVec2f _centerPos, _mainPos, _miniPos, _btnPos;
	ePhotoPrimaryCategory	_category;
	ofxAnimatableFloat	_animMiniPosX[3];
	ofxAnimatableFloat	_animBtn;
	map<ePhotoPrimaryCategory, baseUnit>	_mainUIMap;
	wallMgr* _parentWallMgr;

#pragma region Input
private:
	void enableInput();
	void disableInput();
	void inputRelease(inputEventArgs e) override;
	ofRectangle	getInputArea() override;
#pragma endregion

};