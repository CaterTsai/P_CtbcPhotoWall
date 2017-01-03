#include "renderMgr.h"

//--------------------------------------------------------------
void renderMgr::setup(string thumbPath, string sourcePath)
{
	_thumbPath = thumbPath;
	_sourcePath = sourcePath;
	startThread();
	setupDefault();

	_isSetup = true;
}

//--------------------------------------------------------------
void renderMgr::update()
{
	updateImage();
}

//--------------------------------------------------------------
void renderMgr::drawThumb(stPhotoHeader & photoheader, ofVec2f pos, float width, float height)
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
void renderMgr::drawThumb(stPhotoHeader& photoheader, ofRectangle drawRect)
{
	drawThumb(photoheader, drawRect.getPosition(), drawRect.getWidth(), drawRect.getHeight());
}

//--------------------------------------------------------------
void renderMgr::drawImage(stPhotoHeader& photoheader, ofRectangle drawRect)
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
void renderMgr::updateImage()
{
	if (!_imgNeedUpdate.empty())
	{
		lock();
		auto imgEntry_ = _imgNeedUpdate.front();
		_imgNeedUpdate.pop();
		unlock();

		updateTexture(imgEntry_.img);

		if (imgEntry_.isThumbanil)
		{
			insertToMap(_thumbMap, imgEntry_);
		}
		else
		{
			insertToMap(_sourceMap, imgEntry_);
		}
	}
}

//--------------------------------------------------------------
void renderMgr::updateTexture(ofImage & img)
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
void renderMgr::insertToMap(map<int, imgEntry>& map, imgEntry& entry)
{
	if (map.find(entry.photoId) == map.end())
	{
		map.insert(make_pair(entry.photoId, entry));
	}
}

//--------------------------------------------------------------
void renderMgr::checkSetup()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("checkSetup : need call setup"));
	}
}

#pragma region Default
//--------------------------------------------------------------
void renderMgr::setupDefault()
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
		ofLog(OF_LOG_NOTICE, "[renderMgr::setupDefault]Load default success");
	}	
}

//--------------------------------------------------------------
void renderMgr::drawDefault(ePhotoShape eShape, ofVec2f pos, float width, float height)
{
	_defalutThumb[eShape].draw(pos, width, height);
}
#pragma endregion


#pragma region Thread

//--------------------------------------------------------------
void renderMgr::addImage(int id, string path, bool isThumb)
{
	imgEntry entry_;
	entry_.isThumbanil = isThumb;
	entry_.path = path;
	entry_.photoId = id;

	_imgQueue.push(entry_);
}

//--------------------------------------------------------------
void renderMgr::signal()
{
	_condition.notify_all();
}

//--------------------------------------------------------------
void renderMgr::threadedFunction()
{
	while (isThreadRunning())
	{
		if (!_imgQueue.empty())
		{
			lock();
			imgEntry imgEntry_ = _imgQueue.front();
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
renderMgr::renderMgr()
{}

//--------------------------------------------------------------
renderMgr* renderMgr::_pInstance = nullptr;
renderMgr * renderMgr::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new renderMgr();
	}
	return _pInstance;
}

//--------------------------------------------------------------
void renderMgr::Destroy()
{
	if (_pInstance == nullptr)
	{
		delete _pInstance;
	}
}
#pragma endregion
