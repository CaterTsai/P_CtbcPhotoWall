#pragma once

#include "constParameter.h"

class renderMgr : ofThread
{
private:
	struct imgEntry {
		bool isThumbanil;
		ofImage img;
		string path;
		int photoId;
	};

public:
	void setup(string thumbPath, string sourcePath);
	void update();

	void drawThumb(stPhotoHeader& photoheader, ofVec2f pos, float width, float height);
	void drawThumb(stPhotoHeader& photoheader, ofRectangle drawRect);
	void drawImage(stPhotoHeader& photoheader, ofRectangle drawRect);

private:
	void updateImage();
	void updateTexture(ofImage& img);
	void insertToMap(map<int, imgEntry>& map, imgEntry& entry);
	void checkSetup();
private:
	bool _isSetup;
	string _thumbPath, _sourcePath;
	queue<imgEntry> _imgNeedUpdate;
	map<int, imgEntry>	_thumbMap;
	map<int, imgEntry>	_sourceMap;

#pragma region Default
private:
	void setupDefault();
	void drawDefault(ePhotoShape eShape, ofVec2f pos, float width, float height);
private:
	map<ePhotoShape, ofImage>	_defalutThumb;
#pragma endregion


#pragma region Event
public:
	ofEvent<renderEvent>	onLoadFinish;
#pragma endregion

#pragma region Thread
public:
	void addImage(int id, string path, bool isThumb);
	void signal();

private:
	virtual void threadedFunction() override;

	queue<imgEntry>	_imgQueue;
	ofMutex _mutex;

	std::condition_variable	_condition;
#pragma endregion	

#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	renderMgr();
	~renderMgr() { 
		if (isThreadRunning())
		{
			stopThread();
		}
	};
	void operator=(renderMgr const&) {};

public:
	static renderMgr* GetInstance();
	static void Destroy();

private:
	static renderMgr *_pInstance;
#pragma endregion
};