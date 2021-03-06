#pragma once

#include "constParameter.h"

class photoRender : ofThread
{
private:
	struct photoEntry {
		bool isThumbanil;
		ofImage img;
		string path;
		int photoId;
		float aliveT;
	};

public:
	void setup();
	void update(ofEventArgs&);
	
private:	
	void checkSetup();
private:
	bool _isSetup;	

#pragma region Photo
public:
	void drawThumb(stPhotoHeader& photoheader, ofVec3f pos, float width, float height);
	void drawThumb(stPhotoHeader& photoheader, ofRectangle drawRect);
	void drawPhoto(stPhotoHeader& photoheader, ofVec3f pos, float width, float height);
	void drawPhoto(stPhotoHeader& photoheader, ofRectangle drawRect);

private:
	void updateImage();
	void updateTexture(ofImage& img);
	void checkSourceAlive(float delta);
	void checkSignal(float delta);
	void insertToMap(map<int, photoEntry>& map, photoEntry& entry);
private:
	float _mainTimer, _singnalTimer;
	queue<photoEntry> _imgNeedUpdate;
	map<int, photoEntry>	_thumbMap;
	map<int, photoEntry>	_sourceMap;

#pragma region Default
private:
	void setupDefault();
	void drawDefault(ePhotoShape eShape, ofVec2f pos, float width, float height);
private:
	map<ePhotoShape, ofImage>	_defalutThumb;
#pragma endregion

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
	
	bool checkInQueue(int id, bool isThumb);
	virtual void threadedFunction() override;

	queue<photoEntry>	_imgQueue;
	set<int> _thumbChecker, _sourceChecker;
	ofMutex _mutex;

	bool _canSignal;
	std::condition_variable	_condition;
#pragma endregion	

#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	photoRender();
	~photoRender() { 
		if (isThreadRunning())
		{
			stopThread();
		}
	};
	void operator=(photoRender const&) {};

public:
	static photoRender* GetInstance();
	static void Destroy();

private:
	static photoRender *_pInstance;
#pragma endregion
};