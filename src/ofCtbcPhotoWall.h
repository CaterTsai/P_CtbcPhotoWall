#pragma once

#include "constParameter.h"
#include "dataHolder.h"
#include "renderMgr.h"

#include "wallMgr.h"
class ofCtbcPhotoWall : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
		
private:
	float _timer;

#pragma region Photo Wall
public:
	void setupWallMgr();
	void updateWallMgr(float delta);
	void drawWallMgr();
private:
	wallMgr	_photoWall[cCategoryNum];
#pragma endregion

};
