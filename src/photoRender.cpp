#include "photoRender.h"

//--------------------------------------------------------------
void photoRender::setup()
{
	startThread();
	setupDefault();	
	ofAddListener(ofEvents().update, this, &photoRender::update);

	_mainTimer = ofGetElapsedTimef();
	_isSetup = true;
}

//--------------------------------------------------------------
void photoRender::update(ofEventArgs& e)
{
	float delta_ = ofGetElapsedTimef() - _mainTimer;
	_mainTimer += delta_;

	updateImage();
	checkSignal(delta_);

}

//--------------------------------------------------------------
void photoRender::checkSetup()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("checkSetup : need call setup"));
	}
}

#pragma region Photo
//--------------------------------------------------------------
void photoRender::drawThumb(stPhotoHeader & photoheader, ofVec3f pos, float width, float height)
{
	checkSetup();
	auto Iter_ = _thumbMap.find(photoheader.id);

	if (Iter_ != _thumbMap.end() && Iter_->second.img.isAllocated())
	{
		Iter_->second.img.draw(pos, width, height);
	}
	else
	{
		if (!checkInQueue(photoheader.id, true) && Iter_ == _thumbMap.end())
		{
			addImage(photoheader.id, photoheader.thumbnailPath, true);
		}
		drawDefault(photoheader.shape, pos, width, height);
	}
}

//--------------------------------------------------------------
void photoRender::drawThumb(stPhotoHeader& photoheader, ofRectangle drawRect)
{
	drawThumb(photoheader, drawRect.getPosition(), drawRect.getWidth(), drawRect.getHeight());
}

//--------------------------------------------------------------
void photoRender::drawPhoto(stPhotoHeader & photoheader, ofVec3f pos, float width, float height)
{
	checkSetup();
	auto Iter_ = _sourceMap.find(photoheader.id);

	if (Iter_ != _sourceMap.end() && Iter_->second.img.isAllocated())
	{
		Iter_->second.img.draw(pos, width, height);
	}
	else
	{
		if (!checkInQueue(photoheader.id, false) && Iter_ == _sourceMap.end())
		{
			addImage(photoheader.id, photoheader.sourcePath, false);
		}
		drawThumb(photoheader, pos, width, height);
	}
}

//--------------------------------------------------------------
void photoRender::drawPhoto(stPhotoHeader& photoheader, ofRectangle drawRect)
{
	drawPhoto(photoheader, drawRect.getPosition(), drawRect.getWidth(), drawRect.getHeight());
}

//--------------------------------------------------------------
void photoRender::updateImage()
{
	if (!_imgNeedUpdate.empty())
	{
		lock();
		auto photoEntry_ = _imgNeedUpdate.front();
		_imgNeedUpdate.pop();
		unlock();

		updateTexture(photoEntry_.img);

		if (photoEntry_.isThumbanil)
		{
			insertToMap(_thumbMap, photoEntry_);
			_thumbChecker.erase(photoEntry_.photoId);
		}
		else
		{
			insertToMap(_sourceMap, photoEntry_);
			_sourceChecker.erase(photoEntry_.photoId);
		}
	}
}

//--------------------------------------------------------------
void photoRender::updateTexture(ofImage & img)
{
	const ofPixels& pix = img.getPixelsRef();
	img.getTextureReference().allocate(
		pix.getWidth()
		, pix.getHeight()
		, ofGetGlInternalFormat(pix)
	);

	img.setUseTexture(true);
	img.update();
}

//--------------------------------------------------------------
void photoRender::checkSignal(float delta)
{
	if (_imgQueue.size() > 0 && _canSignal)
	{
		_singnalTimer -= delta;

		if (_singnalTimer <= 0)
		{
			signal();
			_singnalTimer = cSingnalCheckTime;
		}
	}
}

//--------------------------------------------------------------
void photoRender::insertToMap(map<int, photoEntry>& map, photoEntry& entry)
{
	if (map.find(entry.photoId) == map.end())
	{
		map.insert(make_pair(entry.photoId, entry));
	}
}
#pragma endregion

#pragma region Default
//--------------------------------------------------------------
void photoRender::setupDefault()
{
	ofImage nh_, nv_, wh_, wv_;
	bool result_ = true;
	result_ &= nh_.load("images/NHDefault.jpg");
	result_ &= nv_.load("images/NVDefault.jpg");
	result_ &= wh_.load("images/WHDefault.jpg");
	result_ &= wv_.load("images/WVDefault.jpg");

	if (result_)
	{
		_defalutThumb[ePhotoNormalHorizontal] = nh_;
		_defalutThumb[ePhotoNormalVertical] = nv_;
		_defalutThumb[ePhotoWideHorizontal] = wh_;
		_defalutThumb[ePhotoWideVertical] = wv_;
		ofLog(OF_LOG_NOTICE, "[photoRender::setupDefault]Load default success");
	}	
}

//--------------------------------------------------------------
void photoRender::drawDefault(ePhotoShape eShape, ofVec2f pos, float width, float height)
{
	_defalutThumb[eShape].draw(pos, width, height);
}
#pragma endregion


#pragma region Thread
//--------------------------------------------------------------
void photoRender::addImage(int id, string path, bool isThumb)
{
	photoEntry entry_;
	entry_.isThumbanil = isThumb;
	entry_.path = path;
	_thumbChecker.insert(id);
	entry_.photoId = id;
	
	_imgQueue.push(entry_);
}

//--------------------------------------------------------------
void photoRender::signal()
{
	if (_canSignal)
	{
		_condition.notify_all();
		_canSignal = false;
	}
	
}

//--------------------------------------------------------------
bool photoRender::checkInQueue(int id, bool isThumb)
{
	if (isThumb)
	{
		return _thumbChecker.find(id) != _thumbChecker.end();
	}
	else
	{
		return _sourceChecker.find(id) != _sourceChecker.end();
	}
	
}

//--------------------------------------------------------------
void photoRender::threadedFunction()
{
	while (isThreadRunning())
	{
		if (!_imgQueue.empty())
		{
			lock();
			photoEntry imgEntry_ = _imgQueue.front();
			_imgQueue.pop();
			
			unlock();

			if (imgEntry_.img.loadImage(imgEntry_.path))
			{
				lock();
				_imgNeedUpdate.push(imgEntry_);
				unlock();
			}
		}
		else
		{
			_canSignal = true;
			std::unique_lock<std::mutex>	lock_(_mutex);
			_condition.wait(lock_);
			
		}
	}
}
#pragma endregion

#pragma region Singleton
//--------------------------------------------------------------
photoRender::photoRender()
	:_isSetup(false)
	,_canSignal(true)
	, _singnalTimer(cSingnalCheckTime)
{}

//--------------------------------------------------------------
photoRender* photoRender::_pInstance = nullptr;
photoRender * photoRender::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new photoRender();
	}
	return _pInstance;
}

//--------------------------------------------------------------
void photoRender::Destroy()
{
	if (_pInstance == nullptr)
	{
		delete _pInstance;
	}
}
#pragma endregion
