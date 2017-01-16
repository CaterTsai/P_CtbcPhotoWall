#pragma once

#include "ofMain.h"

class AudioMgr
{
public:
	void addAduio(string strName, string strFilepath, bool bIsLoop = false, bool bIsMultiPlay = true, float fVol = 1.0);
	void addBGM(string strName, string strFilepath);
	void playAudio(string strName);
	void stopAudio(string strName);

private:
	AudioMgr(){};
	~AudioMgr()
	{
		AudioMgr::Destroy();
	}
	AudioMgr(AudioMgr const&);
	void operator=(AudioMgr const&);

private:
	map<string, ofSoundPlayer>	_AudioMap;

//-------------------
//Singleton
//-------------------
public:
	static AudioMgr* GetInstance();
	static void Destroy();

private:
	static AudioMgr *pInstance;
};