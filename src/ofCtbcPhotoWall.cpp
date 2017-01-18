#include "ofCtbcPhotoWall.h"

//--------------------------------------------------------------
void ofCtbcPhotoWall::setup()
{
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	
	photoRender::GetInstance()->setup("thumbnail/", "");
	dataHolder::GetInstance()->setup("");
	fontMgr::GetInstance()->setup("fonts/");
	inputEventMgr::GetInstance()->enableInput();

	setupImageRender("images/");
	setupAudio();

#ifndef _DEBUG
	ofSetWindowPosition(0, 0);
#endif // !_DEBUG

	AudioMgr::GetInstance()->playAudio(NAME_MGR::BGM_1);
	setupWallMgr();
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::update()
{
	float delta_ = ofGetElapsedTimef() - _timer;
	_timer += delta_;

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
			outIdle();
			break;
		}
		case 'w':
		{
			inIdle();
			break;
		}
		case 'e':
		{
			mainUIout();
			break;
		}
		case '1':
		{
			AudioMgr::GetInstance()->stopAudio(NAME_MGR::BGM_2);
			AudioMgr::GetInstance()->playAudio(NAME_MGR::BGM_1);
			break;
		}
		case '2':
		{
			AudioMgr::GetInstance()->stopAudio(NAME_MGR::BGM_1);
			AudioMgr::GetInstance()->playAudio(NAME_MGR::BGM_2);
			break;
		}
	}
}

#pragma region Image Render
//--------------------------------------------------------------
void ofCtbcPhotoWall::setupImageRender(string path)
{
	imageRender::GetInstance()->setup(path);

	imageRender::GetInstance()->addImage(NAME_MGR::I_Gradient, "gradient.png");
	
}

#pragma endregion

#pragma region Audio Manager
//--------------------------------------------------------------
void ofCtbcPhotoWall::setupAudio()
{
	AudioMgr::GetInstance()->addBGM(NAME_MGR::BGM_1, "sounds/bgm1.mp3");
	AudioMgr::GetInstance()->addBGM(NAME_MGR::BGM_2, "sounds/bgm2.mp3");
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
	setupIdleVideo("videos/idle.avi");

	_blurLevel.reset(1.0f);
	_wallState = ePhotoWall_Idle;
	_idleVideo.play();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::updateWallMgr(float delta)
{
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].update(delta);
	}
	
	updateWallBlur(delta);
	updateIdleVideo();
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


		drawIdleVideo();
		ofPopStyle();
	}
}


//--------------------------------------------------------------
void ofCtbcPhotoWall::inIdle()
{
	if (_wallState == ePhotoWall_Play)
	{
		mainUIout();
		_idleVideo.play();
		_idleVideo.setFrame(0);
		_idleVideo.update();

		_wallState = ePhotoWall_BlurIn;
		_blurLevel.animateTo(1.0f);
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::outIdle()
{
	if (_wallState == ePhotoWall_Idle)
	{
		_wallState = ePhotoWall_BlurOut;
		_blurLevel.animateTo(0.0f);
		
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::mainUIin()
{
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].mainUIin();
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::mainUIout()
{
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].mainUIout();
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
				
				_idleVideo.stop();
				mainUIin();
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
	
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].draw();
	}

	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].drawShadow();
	}

	
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].drawSelect();
	}
	ofPopStyle();
}

#pragma region Idle Video
//--------------------------------------------------------------
void ofCtbcPhotoWall::setupIdleVideo(string path)
{
	if (!_idleVideo.load(path))
	{
		ofLog(OF_LOG_ERROR, "[ofCtbcPhotoWall::setupIdleVideo]load video faield");
	}
	_idleVideo.setLoopState(ofLoopType::OF_LOOP_NORMAL);
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::updateIdleVideo()
{
	if (_wallState != ePhotoWall_Play && _idleVideo.isLoaded())
	{
		_idleVideo.update();
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::drawIdleVideo()
{
	if (_wallState != ePhotoWall_Play && _idleVideo.isLoaded())
	{
		ofSetColor(255, _blurLevel.getCurrentValue() * 255);
		_idleVideo.draw(0, 0);
	}
}
#pragma endregion

#pragma endregion

