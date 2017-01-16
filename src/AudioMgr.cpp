#include "AudioMgr.h"

//--------------------------------------------------------------
void AudioMgr::addAduio(string strName, string strFilepath, bool bIsLoop, bool bIsMultiPlay, float fVol)
{
	ofSoundPlayer NewSoundPlayer_;
	if(!NewSoundPlayer_.loadSound(strFilepath))
	{
		ofLog(OF_LOG_ERROR, "[AudioMgr]Can't found sound file : " + strFilepath);
		return;
	}
	NewSoundPlayer_.setLoop(bIsLoop);
	NewSoundPlayer_.setMultiPlay(bIsMultiPlay);
	NewSoundPlayer_.setVolume(fVol);
	try
	{
		_AudioMap.insert(make_pair(strName, NewSoundPlayer_));
	}
	catch(const exception& e)
	{
		ofLog(OF_LOG_ERROR, "[AudioMgr]Insert sound player to map faild : " + ofToString(e.what()));
	}
}

//--------------------------------------------------------------
void AudioMgr::addBGM(string strName, string strFilepath)
{
	this->addAduio(strName, strFilepath, true, false, 0.5);
}

//--------------------------------------------------------------
void AudioMgr::playAudio(string strName)
{
	auto Iter_ = _AudioMap.find(strName);
	if(Iter_ == _AudioMap.end())
	{
		ofLog(OF_LOG_WARNING, "[AudioMgr]Can't found this audio in map :" + strName);
		return;
	}
	Iter_->second.play();
}

//--------------------------------------------------------------
void AudioMgr::stopAudio(string strName)
{
	auto Iter_ = _AudioMap.find(strName);
	if(Iter_ == _AudioMap.end())
	{
		ofLog(OF_LOG_WARNING, "[AudioMgr]Can't found this audio in map :" + strName);
		return;
	}
	Iter_->second.stop();
}

//--------------------------------------------------------------
AudioMgr* AudioMgr::pInstance = 0;
AudioMgr* AudioMgr::GetInstance()
{
	if(pInstance == 0)
	{
		pInstance = new AudioMgr();
	}
	return pInstance;
}

//--------------------------------------------------------------
void AudioMgr::Destroy()
{
	if(pInstance != 0)
	{
		delete pInstance;
	}
}