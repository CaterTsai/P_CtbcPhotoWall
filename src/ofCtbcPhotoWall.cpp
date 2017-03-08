#include "ofCtbcPhotoWall.h"

//--------------------------------------------------------------
void ofCtbcPhotoWall::setup()
{
	_displayMsg = false;
	
	ofBackground(0);
	ofSetVerticalSync(false);
	ofSetFrameRate(60);
		
	configMgr::load("config/_config.xml");
	photoRender::GetInstance()->setup();
	
	ofAddListener(dataHolder::GetInstance()->_onSetupFinish, this, &ofCtbcPhotoWall::onDataHolderLoadFinish);
	dataHolder::GetInstance()->setup();
	fontMgr::GetInstance()->setup("fonts/");

	inputEventMgr::GetInstance()->enableInput();
	ofAddListener(inputEventMgr::GetInstance()->_onIdleTrigger, this, &ofCtbcPhotoWall::onIdleTrigger);
	
	setupImageRender("images/");
	setupAudio();

	while (true)
	{
		if (_canSetup)
		{
			setupAfterDataHolder();
			break;
		}
		Sleep(500);
	}
#ifndef _DEBUG
	ofHideCursor();
	ofToggleFullscreen();
#endif // !_DEBUG
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::setupAfterDataHolder()
{
	AudioMgr::GetInstance()->playAudio(NAME_MGR::BGM);
	setupWallMgr();
	enableInput();
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::update()
{
	float delta_ = ofGetElapsedTimef() - _timer;
	_timer += delta_;

	updateWallMgr(delta_);
	dataHolder::GetInstance()->update(delta_);
	inputEventMgr::GetInstance()->idleCheck(delta_);
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::draw()
{
	if (!dataHolder::GetInstance()->setupCheck())
	{
		return;
	}

	drawWallMgr();

	if (_displayMsg)
	{
		ofPushStyle();
		ofSetColor(255, 0, 0);
		{
			ofLine(0, cWindowHeight / 3, cWindowWidth, cWindowHeight / 3);
			ofLine(0, (cWindowHeight * 2) / 3, cWindowWidth, (cWindowHeight * 2) / 3);
		}

#ifndef USE_MOUSE
		inputEventMgr::GetInstance()->displayTouch();
#endif // USE_MOUSE
		ofDrawBitmapStringHighlight("FPS:" + ofToString(ofGetFrameRate()), 0, 100);
		ofPopStyle();
	}	
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::keyPressed(int key)
{
	switch (key)
	{
		case 'w':
		{
			inIdle();
			break;
		}
		case 'd':
		{
			_displayMsg ^= true;
			_displayMsg ? ofShowCursor() : ofHideCursor();
			break;
		}
		case 'f':
		{
			ofToggleFullscreen();
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

	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnZH1, "btnZH_1.png");
	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnEN1, "btnEN_1.png");
	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnZH2, "btnZH_2.png");
	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnEN2, "btnEN_2.png");
	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnZH3, "btnZH_3.png");
	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnEN3, "btnEN_3.png");
	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnZH4, "btnZH_4.png");
	imageRender::GetInstance()->addImage(NAME_MGR::I_BtnEN4, "btnEN_4.png");
	
}

#pragma endregion

#pragma region Audio Manager
//--------------------------------------------------------------
void ofCtbcPhotoWall::setupAudio()
{
	AudioMgr::GetInstance()->addAduio(NAME_MGR::BTN_CLICK, "sounds/click.wav");
	AudioMgr::GetInstance()->addBGM(NAME_MGR::BGM, "sounds/bgm.mp3");
}
#pragma endregion

#pragma region Photo Wall
//--------------------------------------------------------------
void ofCtbcPhotoWall::setupWallMgr()
{
	int startX_ = 0;
	
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		ePhotoPrimaryCategory eCategory_ = (ePhotoPrimaryCategory)(cCategoryNum - idx_ - 1);
		_photoWall[idx_].setup(
			eCategory_
			,ofRectangle(idx_ * cPhotoWallCategoryWidth, 0, cPhotoWallCategoryWidth, cWindowHeight)
		);

		_photoWall[idx_].addWallList(cMinimumPhotoWidth);
		_photoWall[idx_].addWallList(cMinimumPhotoWidth * 2, true);		
		_photoWall[idx_].addWallList(cMinimumPhotoWidth);
		_photoWall[idx_].addWallList(cMinimumPhotoWidth);
		_photoWall[idx_].addWallList(cMinimumPhotoWidth);
		_photoWall[idx_].addWallList(cMinimumPhotoWidth);
		_photoWall[idx_].addWallList(cMinimumPhotoWidth * 2);
		_photoWall[idx_].addWallList(cMinimumPhotoWidth);
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
		endAll();
		_idleVideo.play();
		//_idleVideo.setFrame(0);
		//_idleVideo.update();

		_wallState = ePhotoWall_BlurIn;
		_blurLevel.animateTo(1.0f);
		enableInput();
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::outIdle()
{
	if (_wallState == ePhotoWall_Idle)
	{
		_wallState = ePhotoWall_BlurOut;
		_blurLevel.animateTo(0.0f);
		disableInput();
		inputEventMgr::GetInstance()->enalbeIdleCheck();
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::startAll()
{
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].start();
	}
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::endAll()
{
	for (int idx_ = 0; idx_ < cCategoryNum; idx_++)
	{
		_photoWall[idx_].end();
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
				startAll();

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
	_idleVideo.setLoopState(ofLoopType::OF_LOOP_NONE);
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::updateIdleVideo()
{
	if (_wallState != ePhotoWall_Play && _idleVideo.isLoaded())
	{
		_idleVideo.update();

		if (_idleVideo.getIsMovieDone())
		{
			outIdle();
		}
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

#pragma region input
//--------------------------------------------------------------
void ofCtbcPhotoWall::enableInput()
{
	inputEventMgr::GetInstance()->registerInputEvent(this, eInputTop);
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::disableInput()
{
	inputEventMgr::GetInstance()->unregisterInputEvent(this);
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::inputRelease(inputEventArgs e)
{
	outIdle();
}

//--------------------------------------------------------------
ofRectangle ofCtbcPhotoWall::getInputArea()
{
	return ofRectangle(0, 0, cWindowWidth, cWindowHeight);
}

//--------------------------------------------------------------
void ofCtbcPhotoWall::onIdleTrigger()
{
	inIdle();
}

#pragma endregion

#pragma region Data Holder
//--------------------------------------------------------------
void ofCtbcPhotoWall::onDataHolderLoadFinish()
{
	_canSetup = true;
}
#pragma endregion

