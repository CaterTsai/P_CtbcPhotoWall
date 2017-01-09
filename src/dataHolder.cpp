#include "dataHolder.h"

//--------------------------------------------------------------
void dataHolder::setup(string url)
{
	_backendUrl = url;
	loadPhotoHeader();

}

//--------------------------------------------------------------
stPhotoHeader & dataHolder::getPhotoHeader(int photoId)
{	
	setupCheck();
	auto Iter_ = _photoMap.find(photoId);
	if (Iter_ != _photoMap.end())
	{
		return Iter_->second;
	}
	else
	{
		throw std::runtime_error(("getPhotoHeader : can't photo"));
	}
}

//--------------------------------------------------------------
stPhotoData dataHolder::getPhotoData(int photoId)
{
	setupCheck();
	return stPhotoData();
}
//--------------------------------------------------------------
vector<int> dataHolder::getPhotoID(PHOTO_TYPE type)
{
	setupCheck();
	auto Iter_ = _typeToPhotoID.find(type);
	if (Iter_ != _typeToPhotoID.end())
	{
		return Iter_->second;
	}
	else
	{
		throw std::runtime_error(("getPhotoID : can't found photo type "));
	}
}

//--------------------------------------------------------------
vector<int> dataHolder::getPhotoID(ePhotoPrimaryCategory eCategory)
{
	setupCheck();
	auto Iter_ = _categoryToPhotoID.find(eCategory);
	if (Iter_ != _categoryToPhotoID.end())
	{
		return Iter_->second;
	}
	else
	{
		throw std::runtime_error(("getPhotoID : can't found ePhotoPrimaryCategory"));
	}
}

//--------------------------------------------------------------
void dataHolder::setupCheck()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("setupCheck : need setup"));
	}
}

//--------------------------------------------------------------
void dataHolder::loadPhotoHeader()
{
	_isSetup = true;

	//Debug
	ofDirectory dir_("thumbnail");
	dir_.allowExt("jpg");
	dir_.listDir();
	for (int idx_ = 0; idx_ < dir_.numFiles(); idx_++)
	{
		string fileName_ = dir_.getName(idx_);
		int imageData_ = stoi(fileName_.substr(0, 10));
		stPhotoHeader header_;
		header_.id = imageData_ & PHOTO_ID_MASK;
		header_.shape = (ePhotoShape)( (imageData_ & PHOTO_SHAPE_MASK) >> 16);
		header_.type = (imageData_ & PHOTO_TYPE_MASK) >> 20;
		header_.category = (ePhotoPrimaryCategory)((imageData_ & PHOTO_CATEGORY_MASK) >> 28);
		header_.thumbnailPath = fileName_;

		addPhotoMap(header_);

		
		photoRender::GetInstance()->addImage(header_.id, header_.thumbnailPath, true);
	}

	photoRender::GetInstance()->signal();
}

//--------------------------------------------------------------
void dataHolder::addPhotoMap(stPhotoHeader & photoHeader)
{
	auto iter_ = _photoMap.find(photoHeader.id);

	if (iter_ != _photoMap.end())
	{
		ofLog(OF_LOG_NOTICE,("addPhotoMap : photoid already exist"));
	}
	else
	{
		_photoMap.insert(make_pair(photoHeader.id, photoHeader));
		addType2PhotoID(photoHeader.type, photoHeader.id);
		addCategory2PhotoID(photoHeader.category, photoHeader.id);
	}
	
}

//--------------------------------------------------------------
void dataHolder::addType2PhotoID(PHOTO_TYPE type, int photoid)
{
	auto iter_ = _typeToPhotoID.find(type);

	if (iter_ == _typeToPhotoID.end())
	{
		vector<int> photoID_;
		_typeToPhotoID.insert(make_pair(type, photoID_));
		iter_ = _typeToPhotoID.find(type);
	}

	iter_->second.push_back(photoid);
}

//--------------------------------------------------------------
void dataHolder::addCategory2PhotoID(ePhotoPrimaryCategory eCategory, int photoid)
{
	auto iter_ = _categoryToPhotoID.find(eCategory);

	if (iter_ == _categoryToPhotoID.end())
	{
		vector<int> photoID_;
		_categoryToPhotoID.insert(make_pair(eCategory, photoID_));
		iter_ = _categoryToPhotoID.find(eCategory);
	}
	iter_->second.push_back(photoid);
}

#pragma region Singletion
//--------------------------------------------------------------
dataHolder* dataHolder::pInstance = 0;

//--------------------------------------------------------------
dataHolder* dataHolder::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new dataHolder();
	}
	return pInstance;
}

//--------------------------------------------------------------
void dataHolder::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion
