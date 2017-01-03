#pragma once
#include "ofMain.h"

/*
ofxWinTouchHook.
- using 'hooks' to catch touch events before the window processes the events
- no need to modify glfw source with hooks
- tested on win7+8 with OF 0.9

Ref:
- http://www.unknowncheats.me/forum/c-and-c/83707-setwindowshookex-example.html
- http://stackoverflow.com/questions/19776866/c-global-hooking-why-setwindowshookex-return-null
- http://stackoverflow.com/questions/21069643/is-it-possible-to-remove-touch-messages-wm-pointerdown-etc-that-an-applicatio
*/

// toggle this to use only windows 8 wm_pointer events instead of wm_touch
#define USE_WM_POINTER_EVENTS


class ofxWinTouchHook {

	public:
		static void EnableTouch();
		static void DisableTouch();
		
		// making our own events as ofApp is giving us duplicates
		static ofEvent<ofTouchEventArgs> touchDown;
		static ofEvent<ofTouchEventArgs> touchMoved;
		static ofEvent<ofTouchEventArgs> touchUp;
};

template<class ListenerClass>
void ofxRegisterWinTouchEvents(ListenerClass * listener, int prio = OF_EVENT_ORDER_AFTER_APP)
{
	ofAddListener(ofxWinTouchHook::touchDown, listener, &ListenerClass::touchDown, prio);
	ofAddListener(ofxWinTouchHook::touchMoved, listener, &ListenerClass::touchMoved, prio);
	ofAddListener(ofxWinTouchHook::touchUp, listener, &ListenerClass::touchUp, prio);
}

template<class ListenerClass>
void ofxUnregisterWinTouchEvents(ListenerClass * listener, int prio = OF_EVENT_ORDER_AFTER_APP)
{
	ofRemoveListener(ofxWinTouchHook::touchDown, listener, &ListenerClass::touchDown, prio);
	ofRemoveListener(ofxWinTouchHook::touchMoved, listener, &ListenerClass::touchMoved, prio);
	ofRemoveListener(ofxWinTouchHook::touchUp, listener, &ListenerClass::touchUp, prio);
}