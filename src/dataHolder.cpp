#include "dataHolder.h"

//--------------------------------------------------------------
void dataHolder::setup(string url)
{
	_backendUrl = url;
	loadPhotoCategoryName();
	loadPhotoTypeName();
	loadPhotoHeader();
	_isSetup = true;
}

//--------------------------------------------------------------
void dataHolder::setupCheck()
{
	if (!_isSetup)
	{
		throw std::runtime_error(("setupCheck : need setup"));
	}
}

#pragma region Photo Category
//--------------------------------------------------------------
string dataHolder::getCategoryName(ePhotoPrimaryCategory eCategory, bool isZH)
{
	return _categoryName[eCategory].getText(isZH);
}

//--------------------------------------------------------------
textUnit dataHolder::getCategoryName(ePhotoPrimaryCategory eCategory)
{
	return _categoryName[eCategory];
}

//--------------------------------------------------------------
void dataHolder::loadPhotoCategoryName()
{
	//TODO
	_categoryName[ePhotoCategory_1] = textUnit("榮耀足跡", "CTBC Glory");
	_categoryName[ePhotoCategory_2] = textUnit("成長印記", "We are growing");
	_categoryName[ePhotoCategory_3] = textUnit("掌聲響起", "You're my star");
	_categoryName[ePhotoCategory_4] = textUnit("家在一起", "We are family");
}

#pragma endregion

#pragma region Photo Type
//--------------------------------------------------------------
vector<PHOTO_TYPE> dataHolder::getType(ePhotoPrimaryCategory eCategory)
{
	vector<PHOTO_TYPE> typeList_;
	for (auto& iter_ : _photoTypeName[eCategory])
	{
		typeList_.push_back(iter_.first);
	}
	return typeList_;
}
//--------------------------------------------------------------
string dataHolder::getTypeName(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type, bool isZH)
{
	auto iter_ = _photoTypeName[eCategory].find(type);
	if (iter_ != _photoTypeName[eCategory].end())
	{
		return iter_->second.getText(isZH);
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[dataHolder::getTypeName]Can't found PHOTO_TYPE :" + ofToString(type));
		return string();
	}
}

//--------------------------------------------------------------
void dataHolder::loadPhotoTypeName()
{
	//TODO
	for (int idx_ = 0; idx_ < 4; idx_++)
	{
		map<PHOTO_TYPE, textUnit> photoTypeName_;
		for (int pIdx_ = 0; pIdx_ < 10; pIdx_++)
		{
			string nameZH_ = "類:" + ofToString(idx_) + " 項:" + ofToString(pIdx_);
			string nameEN_ = "C:" + ofToString(idx_) + " T:" + ofToString(pIdx_);
			photoTypeName_.insert(make_pair(pIdx_, textUnit(nameZH_, nameEN_)));
		}
		_photoTypeName.insert(make_pair((ePhotoPrimaryCategory)idx_, photoTypeName_));
	}
}
#pragma endregion

#pragma region PhotoHeader
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
vector<int> dataHolder::getPhotoID(ePhotoPrimaryCategory eCategory)
{
	setupCheck();
	vector<int> rVal_;

	for (auto& iter_ : _typeToPhotoID[eCategory])
	{
		rVal_.insert(rVal_.end(), iter_.second.begin(), iter_.second.end());
	}
	return rVal_;
}

//--------------------------------------------------------------
vector<int> dataHolder::getPhotoID(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type)
{
	setupCheck();
	vector<int> rVal_;

	auto& iter_ = _typeToPhotoID[eCategory].find(type);
	if (iter_ != _typeToPhotoID[eCategory].end())
	{
		rVal_.insert(rVal_.end(), iter_->second.begin(), iter_->second.end());
	}
	return rVal_;
}

//--------------------------------------------------------------
void dataHolder::loadPhotoHeader()
{
	//TODO
	ofDirectory dir_("thumbnail");
	dir_.allowExt("jpg");
	dir_.listDir();
	for (int idx_ = 0; idx_ < dir_.numFiles(); idx_++)
	{
		string fileName_ = dir_.getName(idx_);
		int imageData_ = stoi(fileName_.substr(0, 10));
		stPhotoHeader header_;
		header_.id = imageData_ & PHOTO_ID_MASK;
		header_.shape = (ePhotoShape)((imageData_ & PHOTO_SHAPE_MASK) >> 16);
		header_.type = (imageData_ & PHOTO_TYPE_MASK) >> 20;
		header_.category = (ePhotoPrimaryCategory)((imageData_ & PHOTO_CATEGORY_MASK) >> 28);
		header_.thumbnailPath = fileName_;
		header_.sourcePath = fileName_;
		header_.title = "";
		header_.msg = "";

		addPhotoMap(header_);
	}
}

//--------------------------------------------------------------
void dataHolder::addPhotoMap(stPhotoHeader & photoHeader)
{
	auto iter_ = _photoMap.find(photoHeader.id);

	if (iter_ != _photoMap.end())
	{
		ofLog(OF_LOG_NOTICE, ("addPhotoMap : photoid already exist"));
	}
	else
	{
		_photoMap.insert(make_pair(photoHeader.id, photoHeader));
		addIndex(photoHeader.category, photoHeader.type, photoHeader.id);
	}

}

//--------------------------------------------------------------
void dataHolder::addIndex(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type, int photoid)
{
	auto iter_ = _typeToPhotoID[eCategory].find(type);

	if (iter_ == _typeToPhotoID[eCategory].end())
	{
		vector<int> photoID_;
		_typeToPhotoID[eCategory].insert(make_pair(type, photoID_));
		iter_ = _typeToPhotoID[eCategory].find(type);
	}

	iter_->second.push_back(photoid);
}

#pragma endregion

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
