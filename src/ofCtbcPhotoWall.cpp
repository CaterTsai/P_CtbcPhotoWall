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
	switch (key)
	{
		case 'q':
		{
			_wallState = ePhotoWall_BlurOut;
			_blurLevel.animateTo(0.0f);
			break;
		}
		case 'w':
		{
			_wallState = ePhotoWall_BlurIn;
			_blurLevel.animateTo(1.0f);
			break;
		}
	}
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


	setupWallBlur();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::updateWallMgr(float delta)
{
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].update(delta);
	}
	
	updateWallBlur(delta);
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::drawWallMgr()
{
	if (_wallState == ePhotoWall_Play)
	{
		drawPhotoWall();
	}
	else
	{
		ofPushStyle();
		
		_blur.draw();

		ofSetColor(0, _blurLevel.getCurrentValue() * 150);
		ofFill();
		ofRect(0, 0, cWindowWidth, cWindowHeight);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::setupWallBlur()
{
	if (ofIsGLProgrammableRenderer()) {
		_blur.setup("shadersGL3/shaderBlurX", "shadersGL3/shaderBlurY", cWindowWidth, cWindowHeight);
	}
	else {
		_blur.setup("shadersGL2/shaderBlurX", "shadersGL2/shaderBlurY", cWindowWidth, cWindowHeight);
	}

	_blurLevel.setDuration(1.0f);
	_blurLevel.setRepeatType(AnimRepeat::PLAY_ONCE);
	_blurLevel.reset(1.0f);

	_wallState = ePhotoWall_Idle;
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::updateWallBlur(float delta)
{
	if (_wallState == ePhotoWall_Play)
	{
		return;
	}
	_blurLevel.update(delta);


	switch (_wallState)
	{
		case ePhotoWall_BlurOut:
		{
			if (_blurLevel.hasFinishedAnimating() && _blurLevel.getPercentDone() == 1.0f)
			{
				_wallState = ePhotoWall_Play;
			}
			break;
		}
		case ePhotoWall_BlurIn:
		{
			if (_blurLevel.hasFinishedAnimating() && _blurLevel.getPercentDone() == 1.0f)
			{
				_wallState = ePhotoWall_Idle;
			}
			break;
		}
	}

	_blur.begin(_blurLevel.getCurrentValue() * 1.5f);
	{
		drawPhotoWall();
	}
	_blur.end();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::drawPhotoWall()
{
	ofPushStyle();
	ofSetColor(255);
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

	ofPopStyle();
}
#pragma endregion

