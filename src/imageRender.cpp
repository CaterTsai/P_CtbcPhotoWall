#include "imageRender.h"

//--------------------------------------------------------------
void imageRender::setup(string imgFolderPath)
{
	_imgPath = imgFolderPath;
}


//--------------------------------------------------------------
void imageRender::checkSetup()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("checkSetup : need call setup"));
	}
}

#pragma region Image
//--------------------------------------------------------------
void imageRender::addImage(string name, string path)
{
	checkSetup();

	ofImage newImg_;
	if (newImg_.load(_imgPath + path))
	{
		_imgMap.insert(make_pair(name, newImg_));
	}
	else
	{
		throw std::runtime_error(string("addImage : load image faield") + name);
	}
}

//--------------------------------------------------------------
void imageRender::drawImage(string name, ofVec3f pos, float width, float height)
{
	checkSetup();

	auto Iter_ = _imgMap.find(name);

	if (Iter_ != _imgMap.end())
	{
		Iter_->second.draw(pos, width, height);
	}
}

//--------------------------------------------------------------
void imageRender::drawImage(string name, ofRectangle rect)
{
	drawImage(name, rect.position, rect.getWidth(), rect.getHeight());
}

#pragma endregion


#pragma region Singleton
//--------------------------------------------------------------
imageRender::imageRender()
{}

//--------------------------------------------------------------
imageRender* imageRender::_pInstance = nullptr;
imageRender * imageRender::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new imageRender();
	}
	return _pInstance;
}

//--------------------------------------------------------------
void imageRender::Destroy()
{
	if (_pInstance == nullptr)
	{
		delete _pInstance;
	}
}
#pragma endregion


