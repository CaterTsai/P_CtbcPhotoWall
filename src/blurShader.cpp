#include "blurShader.h"

//--------------------------------------------------------------
void blurShader::setup(string blurXpath, string blurYpath, int width, int height)
{
	_isSetup = true;
	_isSetup &= _shaderBlurX.load(blurXpath);
	_isSetup &= _shaderBlurY.load(blurYpath);

	if (_isSetup)
	{
		_fboBlurOnePass.allocate(width, height);
		_fboBlurTwoPass.allocate(width, height);
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[blurShader::setup]load shader failed");
	}
}

//--------------------------------------------------------------
void blurShader::draw()
{
	if (!_isSetup)
	{
		return;
	}
	ofPushStyle();
	ofSetColor(255);
	{
		_fboBlurTwoPass.draw(0, 0);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void blurShader::begin(float blurLevel)
{
	if (!_isSetup)
	{
		return;
	}

	if (_isBegin)
	{
		ofLog(OF_LOG_WARNING, "[blurShader::begin]begin and end not match");
		return;
	}

	_isBegin = true;
	_blurLevel = MIN(MAX(blurLevel, 0.0f), 1.5f);

	_fboBlurOnePass.begin();
	ofClear(ofColor(0));
	_shaderBlurX.begin();
	_shaderBlurY.setUniform1f("blurAmnt", _blurLevel);
}

//--------------------------------------------------------------
void blurShader::end()
{
	if (!_isSetup)
	{
		return;
	}

	if (!_isBegin)
	{
		ofLog(OF_LOG_WARNING, "[blurShader::end]begin and end not match");
		return;
	}

	_shaderBlurX.end();
	_fboBlurOnePass.end();
	
	_fboBlurTwoPass.begin();
	ofClear(ofColor());
	_shaderBlurY.begin();
	_shaderBlurY.setUniform1f("blurAmnt", _blurLevel);
	{
		_fboBlurOnePass.draw(0, 0);
	}
	_shaderBlurY.end();
	_fboBlurTwoPass.end();

	_blurLevel = 0.0f;
	_isBegin = false;
}
