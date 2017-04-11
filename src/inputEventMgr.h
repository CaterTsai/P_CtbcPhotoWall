#pragma once

#include "constParameter.h"
#include "configMgr.h"
#include "ofxWinTouchHook.h"
#ifdef USE_TUIO
#include "ofxTuioClient.h"
#endif // USE_TUIO

#pragma region Structure
struct inputEventArgs
{
	ofVec2f pos, delta, diffPos;
	float holdTime;
};


#pragma endregion

class inputEvent
{
public:
	virtual void inputPress(inputEventArgs e) {};
	virtual void inputDrag(inputEventArgs e) {};
	virtual void inputRelease(inputEventArgs e) {};
	virtual ofRectangle	getInputArea() { return ofRectangle(); };
};

class inputEventMgr
{
#pragma region inputEventParam
private:
	struct inputEventParam
	{
		inputEventParam()
		{
			clear();
		}
		void clear()
		{
			_pressPos.set(0);
			_dragPos.set(0);
			_delta.set(0);
			_pressTime = 0.0f;
			_pressObj = nullptr;
		}

		inputEvent*	_pressObj;
		ofVec2f _pressPos, _dragPos, _delta;
		float _pressTime;
	};
#pragma endregion

public:
	void registerInputEvent(inputEvent* input, int level = cInputEventLevel - 1);
	void unregisterInputEvent(inputEvent* input);
private:
	bool isInputEventExist(inputEvent* input, int level);
	bool pressCheck(ofVec2f& pos, inputEventParam& param);
	void dragCheck(ofVec2f& pos, inputEventParam& param);
	void releaseCheck(ofVec2f& pos, inputEventParam& param);
private:
	list<inputEvent*>	_inputEventMap[cInputEventLevel];

#ifdef USE_MOUSE
	inputEventParam _inputEventParam;
#else
	map<int, inputEventParam> _inputEventParamMgr;
#endif // USE_MOUSE
	
#pragma region idle check
public:
	void idleCheck(float delta);
	void enalbeIdleCheck();

private:
	void resetIdle();

private:
	bool _isIdleCheck;
	float _timer;

public:
	ofEvent<void> _onIdleTrigger;
#pragma endregion
	
#pragma region Input
public:
	void enableInput();
	void disableInput();

#ifdef USE_MOUSE
public:

	void mousePressed(ofMouseEventArgs& e);
	void mouseDragged(ofMouseEventArgs& e);
	void mouseReleased(ofMouseEventArgs& e);
	void mouseMoved(ofMouseEventArgs& e) {};
	void mouseScrolled(ofMouseEventArgs& e) {};
	void mouseEntered(ofMouseEventArgs& e) {};
	void mouseExited(ofMouseEventArgs& e) {};

#else
public:
	void touchDown(ofTouchEventArgs& e);
	void touchMoved(ofTouchEventArgs& e);
	void touchUp(ofTouchEventArgs& e);
	void displayTouch();

private:
	bool touchDistanceCheck(ofTouchEventArgs& e);
	ofTouchEventArgs modifyTouchPos(ofTouchEventArgs e);


#endif // USE_MOUSE
	



#ifdef USE_TUIO
public:
	
private:
	void setupTUIO();
private:
	ofxTuioClient _tuio;
#endif // USE_TUIO

private:
#pragma endregion

#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	inputEventMgr();
	~inputEventMgr() {};
	void operator=(inputEventMgr const&) {};

public:
	static inputEventMgr* GetInstance();
	static void Destroy();

private:
	static inputEventMgr *_pInstance;
#pragma endregion
};