#pragma once

#include "ofMain.h"
#include <functional>

class renderEvent : public ofEventArgs {
public:
	int photoID;
};