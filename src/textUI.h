#pragma once

#include "constParameter.h"
#include "ofxTrueTypeFontUC.h"

class textUI
{
public:
	textUI();
	~textUI();

	void setup();
	void update(float delta);
	void draw(ofVec2f pos);

private:

};