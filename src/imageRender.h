#pragma once

#include "constParameter.h"

class imageRender
{
public:
	void setup(string imgFolderPath);

private:
	void checkSetup();
private:
	bool _isSetup;

#pragma region Image
public:
	void addImage(string name, string path);
	void drawImage(string name, ofVec3f pos, float width, float height);
	void drawImage(string name, ofRectangle rect);
	
private:
	string _imgPath;
	map<string, ofImage> _imgMap;

#pragma endregion

#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	imageRender();
	~imageRender() {};
	void operator=(imageRender const&) {};

public:
	static imageRender* GetInstance();
	static void Destroy();

private:
	static imageRender *_pInstance;
#pragma endregion
};