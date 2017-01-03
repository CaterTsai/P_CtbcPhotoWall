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
	int id_ = 0;
	int photoType_ = 0;
	for (int category = 0; category < 4; category++)
	{
		vector<int> photoIDCategoryList_;
		for (int photoTypeIdx = 0; photoTypeIdx < 5; photoTypeIdx++)
		{
			photoType_ ++;
			vector<int> photoIDTypeList_;
			for (int idx_ = 0; idx_ < 100; idx_++)
			{
				stPhotoHeader header_;
				header_.category = (ePhotoPrimaryCategory)category;
				header_.shape = (ePhotoShape)(rand() % 4);
				//header_.id = (category << 28) + (photoType_ << 20) + id_;
				header_.id = id_;
				header_.type = photoTypeIdx;
				header_.thumbnailPath = "thumbnail/" + ofToString(id_, 6, '0') + ".jpg";

				_photoMap.insert(make_pair(header_.id, header_));
				renderMgr::GetInstance()->addImage(id_, header_.thumbnailPath, true);


				photoIDTypeList_.push_back(header_.id);
				photoIDCategoryList_.push_back(header_.id);

				id_++;
			}

			_typeToPhotoID[photoType_] = photoIDTypeList_;
		}
		_categoryToPhotoID[(ePhotoPrimaryCategory)category] = photoIDCategoryList_;
	}

	renderMgr::GetInstance()->signal();
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
