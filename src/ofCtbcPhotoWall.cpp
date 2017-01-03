#include "ofCtbcPhotoWall.h"

//--------------------------------------------------------------
void ofCtbcPhotoWall::setup()
{
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	dataHolder::GetInstance()->setup("");
	renderMgr::GetInstance()->setup("", "");
	
	
	setupWallMgr();
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::update()
{
	float delta_ = ofGetElapsedTimef() - _timer;
	_timer += delta_;

	renderMgr::GetInstance()->update();

	updateWallMgr(delta_);

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::draw()
{
	drawWallMgr();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::keyPressed(int key)
{

}

#pragma region Photo Wall
//--------------------------------------------------------------
void ofCtbcPhotoWall::setupWallMgr()
{
	int startX_ = 0;
	
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].setup(
			(ePhotoPrimaryCategory)idx_
			,ofRectangle(idx_ * cPhotoWallCategoryWidth, 0, cPhotoWallCategoryWidth, cWindowHeight)
		);
		_photoWall[idx_].addWallList(40);
		_photoWall[idx_].addWallList(40);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(40);
		_photoWall[idx_].addWallList(40);
		_photoWall[idx_].addWallList(40);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(40);
		_photoWall[idx_].addWallList(40);
		_photoWall[idx_].addWallList(40);
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::updateWallMgr(float delta)
{
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].update(delta);
	}
	
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::drawWallMgr()
{
	ofVec2f drawPos_(0);
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].draw(drawPos_);
		drawPos_.x += cPhotoWallCategoryWidth;
	}
}
	
#pragma endregion

