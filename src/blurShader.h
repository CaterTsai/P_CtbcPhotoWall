#pragma once

#include "constParameter.h"

class blurShader
{
public:
	blurShader()
		:_isSetup(false)
		, _isBegin(false)
		, _blurLevel(1.0)
	{}
	void setup(string blurXpath, string blurYpath, int width, int height);
	void draw();

	void begin(float blurLevel);
	void end();

private:
	bool _isSetup, _isBegin;
	float _blurLevel;
	ofShader _shaderBlurX, _shaderBlurY;
	ofFbo	_fboBlurOnePass, _fboBlurTwoPass;

};