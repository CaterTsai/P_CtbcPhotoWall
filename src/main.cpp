#include "ofMain.h"
#include "ofCtbcPhotoWall.h"

//========================================================================
int main( ){

	//ofSetupOpenGL(cWindowWidth,cWindowHeight,OF_WINDOW);			// <-------- setup the GL context
	//ofGLWindowSettings settings;
	ofGLFWWindowSettings settings;
		
	//settings.setGLVersion(3, 2);
	settings.width = cWindowWidth;
	settings.height = cWindowHeight;
	settings.windowMode = OF_WINDOW;
	ofCreateWindow(settings);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofCtbcPhotoWall());

}
