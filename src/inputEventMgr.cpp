#include "inputEventMgr.h"


//--------------------------------
void inputEventMgr::registerInputEvent(inputEvent * input, int level)
{
	level = MIN(MAX(level, 0), cInputEventLevel - 1);
	_inputEventMap[level].push_back(input);
}

//--------------------------------
void inputEventMgr::unregisterInputEvent(inputEvent * input)
{
	bool isFind_ = false;
	for (int idx_ = 0; idx_ < cInputEventLevel; idx_++)
	{
		auto iter_ = _inputEventMap[idx_].begin();
		while (iter_ != _inputEventMap[idx_].end())
		{
			if ((*iter_) == input)
			{
				iter_ = _inputEventMap[idx_].erase(iter_);
				isFind_ = true;
				break;
			}
			else
			{
				iter_++;
			}
		}

		if (isFind_)
		{
			break;
		}
	}
}

//--------------------------------
bool inputEventMgr::pressCheck(ofVec2f & pos, inputEventParam & param)
{
	bool isTrigger_ = false;
	for (int idx_ = 0; idx_ < cInputEventLevel; idx_++)
	{
		for (auto& iter_ : _inputEventMap[idx_])
		{
			auto inputArea_ = iter_->getInputArea();
			if (inputArea_.inside(pos))
			{
				param.clear();

				inputEventArgs args_;
				args_.pos = pos;
				iter_->inputPress(args_);

				param._pressObj = iter_;
				param._pressPos = param._dragPos = pos;
				param._pressTime = ofGetElapsedTimef();
				isTrigger_ = true;
				break;
			}
		}

		if (isTrigger_)
		{
			break;
		}
	}
	return isTrigger_;
}

//--------------------------------
void inputEventMgr::dragCheck(ofVec2f & pos, inputEventParam & param)
{
	if (param._pressObj)
	{
		inputEventArgs args_;
		ofVec2f delta_ = pos - param._dragPos;
		param._dragPos = pos;
		param._delta = delta_;
		args_.pos = pos;
		args_.diffPos = pos - param._pressPos;
		args_.delta = delta_;
		args_.holdTime = ofGetElapsedTimef() - param._pressTime;

		param._pressObj->inputDrag(args_);
	}
}

//--------------------------------
void inputEventMgr::releaseCheck(ofVec2f & pos, inputEventParam & param)
{
	if (param._pressObj)
	{
		inputEventArgs args_;
		args_.pos = pos;
		args_.delta = param._delta;
		args_.holdTime = ofGetElapsedTimef() - param._pressTime;
		args_.diffPos = pos - param._pressPos;
		param._pressObj->inputRelease(args_);

		param.clear();
	}
}

#pragma region Input
//--------------------------------
void inputEventMgr::enableInput()
{
#ifdef USE_MOUSE
	ofRegisterMouseEvents(this);
#else

#ifdef USE_TUIO
	setupTUIO();
#else
	ofxRegisterWinTouchEvents(this);
#endif //USE_TUIO

#endif // USE_MOUSE
}

//--------------------------------
void inputEventMgr::disableInput()
{
#ifdef USE_MOUSE
	ofUnregisterMouseEvents(this);
#else
	ofxUnregisterWinTouchEvents(this);
#endif // USE_MOUSE
}

#ifdef USE_MOUSE
//--------------------------------
void inputEventMgr::mousePressed(ofMouseEventArgs& e)
{
	pressCheck(e, _inputEventParam);
}

//--------------------------------
void inputEventMgr::mouseDragged(ofMouseEventArgs& e)
{
	dragCheck(e, _inputEventParam);
}

//--------------------------------
void inputEventMgr::mouseReleased(ofMouseEventArgs& e)
{
	releaseCheck(e, _inputEventParam);
}

#else
//--------------------------------
void inputEventMgr::touchDown(ofTouchEventArgs& e)
{
	if (_inputEventParamMgr.find(e.id) != _inputEventParamMgr.end())
	{
		ofLog(OF_LOG_ERROR, "[inputEventMgr::touchDown]touch id duplicate");
		return;
	}

	inputEventParam _newParam;
	ofTouchEventArgs fixArgs_ = modifyTouchPos(e);
	
	if (pressCheck(fixArgs_, _newParam))
	{
		_inputEventParamMgr.insert(make_pair(e.id, _newParam));
	}
}

//--------------------------------
void inputEventMgr::touchMoved(ofTouchEventArgs& e)
{
	if (_inputEventParamMgr.find(e.id) != _inputEventParamMgr.end())
	{
		ofTouchEventArgs fixArgs_ = modifyTouchPos(e);
		dragCheck(fixArgs_, _inputEventParamMgr[e.id]);
	}
}

//--------------------------------
void inputEventMgr::touchUp(ofTouchEventArgs& e)
{
	if (_inputEventParamMgr.find(e.id) != _inputEventParamMgr.end())
	{
		ofTouchEventArgs fixArgs_ = modifyTouchPos(e);
		releaseCheck(fixArgs_, _inputEventParamMgr[e.id]);
		_inputEventParamMgr.erase(e.id);
	}
}

#endif // USE_MOUSE

//--------------------------------
ofTouchEventArgs inputEventMgr::modifyTouchPos(ofTouchEventArgs e)
{
	auto rVal_ = e;

#ifdef USE_TUIO
	rVal_.x = (1.0f - rVal_.x) * cWindowWidth;
	rVal_.y = (1.0f - rVal_.y) * cWindowHeight;
#else
	rVal_ *= cInputToWindow;
#endif // USE_TUIO	
	return rVal_;
}

#ifdef USE_TUIO
//--------------------------------
void inputEventMgr::displayTUIO()
{
	ofPushStyle();
	for (auto& iter_ : _inputEventParamMgr)
	{
		ofColor color_(
			iter_.first * 71 % 255,
			iter_.first * 91 % 255,
			iter_.first * 51 % 255,
			128);

		ofSetColor(color_);
		ofCircle(iter_.second._dragPos, cInputTUIOCircleSize);
	}
	ofPopStyle();
}
//--------------------------------
void inputEventMgr::setupTUIO()
{
	//Connect to Port
	_tuio.connect(cInputTUIOPort);

	//Assign Global TUIO Callback Functions
	ofAddListener(ofEvents().touchDown, this, &inputEventMgr::touchDown);
	ofAddListener(ofEvents().touchUp, this, &inputEventMgr::touchUp);
	ofAddListener(ofEvents().touchMoved, this, &inputEventMgr::touchMoved);
}
#endif // USE_TUIO

#pragma endregion

#pragma region Singleton
//--------------------------------------------------------------
inputEventMgr::inputEventMgr()
{
#ifdef USE_MOUSE
	_inputEventParam.clear();
#endif // USE_MOUSE
}

//--------------------------------------------------------------
inputEventMgr* inputEventMgr::_pInstance = nullptr;
inputEventMgr * inputEventMgr::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new inputEventMgr();
	}
	
	return _pInstance;
}

//--------------------------------------------------------------
void inputEventMgr::Destroy()
{
	if (_pInstance == nullptr)
	{
		delete _pInstance;
	}
}
#pragma endregion


