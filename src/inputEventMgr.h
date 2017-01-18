#pragma once

#include "constParameter.h"

class inputEvent
{
public:
	virtual void inputPress(ofVec2f pos) {};
	virtual void inputDrag(ofVec2f delta) {};
	virtual void inputRelease(ofVec2f pos) {};
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