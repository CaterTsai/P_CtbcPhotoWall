#pragma once

#include "constParameter.h"
#include "inputEventMgr.h"
#include "ofxAnimatableFloat.h"
#include "ofxXmlSettings.h"
#include "ofxTrueTypeFontUC.h"

class wallMgr;
class mainUI : public inputEvent
{
#pragma region CLASS baseUnit
private:
	class baseUnit {
	public:
		baseUnit() {};
		baseUnit(ofImage& znText, ofImage& enText, ofColor c);

		void update(float delta);
		void draw(ofVec2f pos, int width, int height, bool displayZN = true);

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
		ofxAnimatableFloat	_animTextWidth, _animBackplaneWidth;
	};
#pragma endregion
	
public:
	mainUI();
	void setup(wallMgr* wallMgr, string xmlPath, ePhotoPrimaryCategory eCategory);
	void update(float delta);
	void draw(ofVec2f pos);

	void open();
	void close();

private:
	void drawMain();
	void drawMini();

	void setupMiniAnim();
	void miniIn();
	void miniOut();

	void animStateCheck();
	bool loadXml(string xmlPath);
	void createTextImg(string text, ofImage& img);

	static ofColor getBPColor(ePhotoPrimaryCategory category);
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
	ofxTrueTypeFontUC _font;
	ofVec2f _mainPos, _miniPos;
	ePhotoPrimaryCategory	_category;
	ofxAnimatableFloat	_animMiniPosX[3];
	map<ePhotoPrimaryCategory, baseUnit>	_mainUIMap;
	wallMgr* _parentWallMgr;
#pragma region Input
private:
	void setupInput();
	void inputPress(ofVec2f pos) override;
	ofRectangle	getInputArea() override;
#pragma endregion

};