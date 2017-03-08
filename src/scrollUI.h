#pragma once

#include "constParameter.h"
#include "fontMgr.h"
#include "inputEventMgr.h"
#include "dataHolder.h"
#include "textUnit.h"
#include "ofxAnimatableFloat.h"

class wallMgr;
class scrollUI : public inputEvent
{
public:
	scrollUI();
	void setup(wallMgr* parent);
	void update(float delta);
	void draw(ofVec2f pos);

	void updateScroll(ePhotoPrimaryCategory eCategory, bool isZH);
	void open(bool isRight = true, PHOTO_TYPE selectPhotoType = -1);
	void close();

private:
	void drawRight();
	void drawLeft();
	void updateCanvas();
	void animStateCheck();
	void updateTitle(ePhotoPrimaryCategory eCategory);

	ofVec2f getDrawPos();
private:
	enum eAnimState
	{
		eClose = 0
		,eUIIn
		,eOpen
		,eUIOut
	}_eState;
	bool _isSetup, _isZH, _isRight;
	ePhotoPrimaryCategory _eCategory;
	wallMgr*	_parent;
	ofImage	_titleImg;
	ofFbo	_displayCanvas;
	ofxAnimatableFloat	_animRotate;

#pragma region item

#pragma region CLASS  itemUnit
private:
	class itemUnit
	{
	public:
		itemUnit() {}
		itemUnit(PHOTO_TYPE type)
			:_photoType(type)
			, _isSelect(false)
		{}

		void setSelect(bool val);
		void draw(ofVec2f pos, ePhotoPrimaryCategory eCategory, bool isZH);
		PHOTO_TYPE getPhotoType();
	private:
		PHOTO_TYPE	_photoType;
		bool _isSelect;
	};
#pragma endregion

private:
	void drawItemList();
	void updateItemList(ePhotoPrimaryCategory eCategory);
	bool moveDrawPosY(int delta);
	int getSelectIndex(ofVec2f pos);
private:
	int _selectItemIdx;
	int _itemListDrawY, _itemListDrawYMin;
	vector<itemUnit>	_itemList;
#pragma endregion

#pragma region Input
public:
	void enableInput(bool isSelect = false);
	void disableInput();
private:
	void inputPress(inputEventArgs e) override;
	void inputDrag(inputEventArgs e) override;
	void inputRelease(inputEventArgs e) override;
	ofRectangle	getInputArea() override;
#pragma endregion
};