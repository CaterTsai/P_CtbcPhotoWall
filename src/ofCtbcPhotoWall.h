#pragma once

#include "constParameter.h"
#include "dataHolder.h"
#include "photoRender.h"
#include "imageRender.h"
#include "blurShader.h"
#include "wallMgr.h"
#include "fontMgr.h"

#include "ofxDSHapVideoPlayer.h"
#include "AudioMgr.h"

class ofCtbcPhotoWall : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
		
private:
	float _timer;

#pragma region Image Render
private:
	void setupImageRender(string path);
#pragma endregion
	
#pragma region Audio Manager
public:
	void setupAudio();
#pragma endregion
	
#pragma region Photo Wall
public:
	void setupWallMgr();
	void updateWallMgr(float delta);
	void drawWallMgr();

	void inIdle();
	void outIdle();

	void startAll();
	void endAll();

private:
	void setupWallBlur();
	void updateWallBlur(float delta);
	void drawPhotoWall();

private:
	enum ePhotoWallState
	{
		ePhotoWall_Idle	=	0
		,ePhotoWall_BlurOut
		,ePhotoWall_Play
		,ePhotoWall_BlurIn
	}_wallState;
	wallMgr	_photoWall[cCategoryNum];
	blurShader _blur;
	ofxAnimatableFloat	_blurLevel;

#pragma region Idle Video
private:
	void setupIdleVideo(string path);
	void updateIdleVideo();
	void drawIdleVideo();
private:
	ofxDSHapVideoPlayer	_idleVideo;

#pragma endregion


#pragma endregion

#pragma region Debug

#pragma endregion


};
