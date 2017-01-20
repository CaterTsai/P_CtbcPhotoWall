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

#pragma region Input
#ifdef USE_MOUSE
//--------------------------------
void inputEventMgr::mouseDragged(ofMouseEventArgs& e)
{
	if (_pressObj)
	{
		inputEventArgs args_;
		ofVec2f delta_ = e - _dragPos;
		_dragPos = e;
		_delta = delta_;
		args_.pos = e;
		args_.diffPos = e - _pressPos;
		args_.delta = delta_;
		args_.holdTime = ofGetElapsedTimef() - _pressTime;

		_pressObj->inputDrag(args_);
	}
}

//--------------------------------
void inputEventMgr::mousePressed(ofMouseEventArgs& e)
{
	bool isTrigger_ = false;
	for (int idx_ = 0; idx_ < cInputEventLevel; idx_++)
	{
		for (auto& iter_ : _inputEventMap[idx_])
		{
			auto inputArea_ = iter_->getInputArea();
			if (inputArea_.inside(e))
			{
				inputEventArgs args_;
				args_.pos = e;
				iter_->inputPress(args_);

				_pressObj = iter_;
				_dragPos = _pressPos = e;
				_pressTime = ofGetElapsedTimef();
				isTrigger_ = true;
				break;
			}
		}

		if (isTrigger_)
		{
			break;
		}
	}
}

//--------------------------------
void inputEventMgr::mouseReleased(ofMouseEventArgs& e)
{
	if (_pressObj)
	{
		inputEventArgs args_;
		args_.pos = e;
		args_.delta = _delta;
		args_.holdTime = ofGetElapsedTimef() - _pressTime;
		args_.diffPos = e - _pressPos;
		_pressObj->inputRelease(args_);

		_pressPos.set(0);
		_dragPos.set(0);
		_pressObj = nullptr;
		_pressTime = 0.0f;
		_delta.set(0);
	}
}

#else
//--------------------------------
void inputEventMgr::touchDown(ofTouchEventArgs& e)
{
}

//--------------------------------
void inputEventMgr::touchMoved(ofTouchEventArgs& e)
{
}

//--------------------------------
void inputEventMgr::touchUp(ofTouchEventArgs& e)
{
}
#endif // USE_MOUSE
//--------------------------------
void inputEventMgr::enableInput()
{
#ifdef USE_MOUSE
	ofRegisterMouseEvents(this);
#else
	ofxRegisterWinTouchEvents(this);
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
#pragma endregion

#pragma region Singleton
//--------------------------------------------------------------
inputEventMgr::inputEventMgr()
	:_pressObj(nullptr)
	, _pressPos(0)
	, _pressTime(0.0f)
{}

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


