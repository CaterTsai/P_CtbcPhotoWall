#include "photoRender.h"

//--------------------------------------------------------------
void photoRender::setup(string thumbPath, string sourcePath)
{
	_thumbPath = thumbPath;
	_sourcePath = sourcePath;
	startThread();
	setupDefault();	
	ofAddListener(ofEvents().update, this, &photoRender::update);
	_isSetup = true;
}

//--------------------------------------------------------------
void photoRender::update(ofEventArgs&)
{
	updateImage();
	
}

//--------------------------------------------------------------
void photoRender::checkSetup()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("checkSetup : need call setup"));
	}
}

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
		drawDefault(photoheader.shape, pos, width, height);
	}
}

//--------------------------------------------------------------
void photoRender::drawThumb(stPhotoHeader& photoheader, ofRectangle drawRect)
{
	drawThumb(photoheader, drawRect.getPosition(), drawRect.getWidth(), drawRect.getHeight());
}

//--------------------------------------------------------------
void photoRender::drawImage(stPhotoHeader& photoheader, ofRectangle drawRect)
{
	checkSetup();
	auto Iter_ = _sourceMap.find(photoheader.id);

	if (Iter_ != _sourceMap.end() && Iter_->second.img.isAllocated())
	{
		Iter_->second.img.draw(drawRect);
	}
	else
	{
		drawThumb(photoheader, drawRect);
	}
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
		}
		else
		{
			insertToMap(_sourceMap, photoEntry_);
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
void photoRender::insertToMap(map<int, photoEntry>& map, photoEntry& entry)
{
	if (map.find(entry.photoId) == map.end())
	{
		map.insert(make_pair(entry.photoId, entry));
	}
}



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
	if (isThumb)
	{
		entry_.path = _thumbPath + path;
	}
	else
	{
		entry_.path = _sourcePath + path;
	}	
	entry_.photoId = id;

	_imgQueue.push(entry_);
}

//--------------------------------------------------------------
void photoRender::signal()
{
	_condition.notify_all();
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

			if (!imgEntry_.isThumbanil)
			{
				renderEvent event_;
				event_.photoID = imgEntry_.photoId;
				ofNotifyEvent(onLoadFinish, event_, this);
			}
		}
		else
		{
			std::unique_lock<std::mutex>	lock_(_mutex);
			_condition.wait(lock_);
		}
	}
}
#pragma endregion

#pragma region Singleton
//--------------------------------------------------------------
photoRender::photoRender()
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
