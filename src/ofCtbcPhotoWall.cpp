#include "ofCtbcPhotoWall.h"

//--------------------------------------------------------------
void ofCtbcPhotoWall::setup()
{
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	
	photoRender::GetInstance()->setup("thumbnail/", "");
	dataHolder::GetInstance()->setup("");
	setupImageRender("images/");
	
#ifndef _DEBUG
	ofSetWindowPosition(0, 0);
#endif // !_DEBUG


	

	setupWallMgr();
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::update()
{
	float delta_ = ofGetElapsedTimef() - _timer;
	_timer += delta_;

	photoRender::GetInstance()->update();

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

#pragma region Image Render
void ofCtbcPhotoWall::setupImageRender(string path)
{
	imageRender::GetInstance()->setup(path);

	imageRender::GetInstance()->addImage(NAME_MGR::I_Gradient, "gradient.png");
	
}
#pragma endregion



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
		//_photoWall[idx_].addWallList(40);
		//_photoWall[idx_].addWallList(40);
		//_photoWall[idx_].addWallList(80);
		//_photoWall[idx_].addWallList(40);
		//_photoWall[idx_].addWallList(40);
		//_photoWall[idx_].addWallList(40);
		//_photoWall[idx_].addWallList(80);
		//_photoWall[idx_].addWallList(40);
		//_photoWall[idx_].addWallList(40);
		//_photoWall[idx_].addWallList(40);

		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(160);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(160);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(80);
		_photoWall[idx_].addWallList(80);
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

	drawPos_.set(0);
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].drawShadow(drawPos_);
		drawPos_.x += cPhotoWallCategoryWidth;
	}

	drawPos_.set(0);
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].drawSelect(drawPos_);
		drawPos_.x += cPhotoWallCategoryWidth;
	}


}
	
#pragma endregion

