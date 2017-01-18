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
				iter_->inputPress(e);
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


