#pragma once

#include "constParameter.h"
#include "ofxWinTouchHook.h"

struct inputEventArgs
{
	inputEventArgs()
	{
		clear();
	}
	void clear()
	{
		pos.set(0);
		delta.set(0);
		holdTime = 0.0f;
		diffPos.set(0);
	}
	ofVec2f pos, delta, diffPos;
	float holdTime;
};

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
public:
	void registerInputEvent(inputEvent* input, int level = cInputEventLevel - 1);
	void unregisterInputEvent(inputEvent* input);
private:

private:
	list<inputEvent*>	_inputEventMap[cInputEventLevel];
	inputEvent*	_pressObj;
	ofVec2f _pressPos, _dragPos, _delta;
	float _pressTime;
#pragma region Input
#ifdef USE_MOUSE
public:
	void mouseDragged(ofMouseEventArgs& e);
	void mousePressed(ofMouseEventArgs& e);
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
#endif // USE_MOUSE

public:
	void enableInput();
	void disableInput();
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