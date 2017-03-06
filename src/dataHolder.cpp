#include "dataHolder.h"

//--------------------------------------------------------------
void dataHolder::setup()
{	
	setupServer();
	postToServer(NAME_MGR::S_ReqInitData);
	postToServer(NAME_MGR::S_ReqPhotoList);

	loadSmilePhoto();
	_timer = cPhotoSmileCheckTime;
}

//--------------------------------------------------------------
void dataHolder::update(float delta)
{
	checkSmileFile(delta);
}

//--------------------------------------------------------------
bool dataHolder::setupCheck()
{
	return _categorySetup && _typeSetup && _headerSetup;
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
void dataHolder::setPhotoCategoryName(Json::Value & root)
{
	int size_ = root.size();
	for (int idx_ = 0; idx_ < size_; idx_++)
	{
		string zhName_ = root[idx_].get("zhName", 0).asString();
		string enName_ = root[idx_].get("enName", 0).asString();
		_categoryName[(ePhotoPrimaryCategory)idx_] = textUnit(zhName_, enName_);
	}
	_categorySetup = true;
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
PHOTO_TYPE dataHolder::getSmileType()
{
	return _smileType;
}

//--------------------------------------------------------------
void dataHolder::setPhotoTypeName(Json::Value & root)
{
	int size_ = root.size();

	for (int idx_ = 0; idx_ < size_; idx_++)
	{
		int cid_ = stoi(root[idx_].get("cid", "-1").asString());
		int tid_ = stoi(root[idx_].get("tid", "-1").asString());
		string nameZH_ = root[idx_].get("zhName", "").asString();
		string nameEN_ = root[idx_].get("enName", "").asString();


		auto categoryMapIter_ = _photoTypeName.find((ePhotoPrimaryCategory)cid_);
		if (categoryMapIter_ == _photoTypeName.end())
		{
			map<PHOTO_TYPE, textUnit> photoTypeName_;
			_photoTypeName.insert(make_pair((ePhotoPrimaryCategory)cid_, photoTypeName_));
			categoryMapIter_ = _photoTypeName.find((ePhotoPrimaryCategory)cid_);
		}

		auto typeMapIter_ = categoryMapIter_->second.find(tid_);
		if (typeMapIter_ == categoryMapIter_->second.end())
		{
			categoryMapIter_->second.insert(make_pair(tid_, textUnit(nameZH_, nameEN_)));
		}
	}
	_smileType = 2;
	_typeSetup = true;
}
#pragma endregion

#pragma region Smile Photo
//--------------------------------------------------------------
void dataHolder::loadSmilePhoto()
{
	ofDirectory dir_(configMgr::exSmilePath + configMgr::exThumbFolderName);
	dir_.allowExt("jpg");
	dir_.listDir();
	
	_smileBaseID = (ePhotoCategory_4 << 28) + (_smileType << 20) + (ePhotoWideVertical << 16);
	for (int idx_ = 0; idx_ < dir_.numFiles(); idx_++)
	{
		string fileName_ = dir_.getName(idx_);
		stPhotoHeader header_;
		header_.id = _smileBaseID + idx_;
		header_.shape = ePhotoWideVertical;
		header_.type = _smileType;
		header_.category = ePhotoCategory_4;
		header_.thumbnailPath = configMgr::exSmilePath + configMgr::exThumbFolderName + fileName_;
		header_.sourcePath = configMgr::exSmilePath + configMgr::exSourceFolderName + fileName_;
		header_.titleZH = "Smile Photo";

		addPhotoMap(header_);
	}
}

//--------------------------------------------------------------
void dataHolder::checkSmileFile(float delta)
{
	_timer -= delta;

	if (_timer <= 0.0f)
	{
		_timer = cPhotoSmileCheckTime;

		WIN32_FIND_DATAA Fd_;
		HANDLE pH_ = FindFirstFileA((configMgr::exSmilePath + configMgr::exSmileOrderFolderName + "*.order").c_str(), &Fd_);

		if (pH_ != INVALID_HANDLE_VALUE)
		{
			string orderName_ = string(Fd_.cFileName);
			addSmileHeader(orderName_);

			string removeCmd_ = "del " + configMgr::exSmilePath + configMgr::exSmileOrderFolderName + orderName_;
			system(removeCmd_.c_str());
		}
	}

}

//--------------------------------------------------------------
void dataHolder::addSmileHeader(string order)
{
	int dotIdx_ = order.find(".order");
	string fileName_ = order.substr(0, dotIdx_) + ".png";
	
	int newID_ = _smileBaseID + _typeToPhotoID[ePhotoCategory_4][_smileType].size();
	stPhotoHeader header_;
	header_.id = newID_;
	header_.shape = ePhotoWideVertical;
	header_.type = _smileType;
	header_.category = ePhotoCategory_4;
	header_.thumbnailPath = configMgr::exSmilePath + configMgr::exThumbFolderName + fileName_;
	header_.sourcePath = configMgr::exSmilePath + configMgr::exSourceFolderName + fileName_;


	addPhotoMap(header_);
	
	ofNotifyEvent(_onNewSmilePhoto, header_);
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
bool dataHolder::getPhotoText(int photoID, bool isZH, string& title, string& msg)
{
	setupCheck();
	auto Iter_ = _photoMap.find(photoID);
	if (Iter_ == _photoMap.end())
	{
		ofLog(OF_LOG_ERROR, "[dataHolder::getPhotoText]Request unknow photo");
		return false;
	}	
	if (Iter_->second.titleZH == "")
	{	
		postToServer(NAME_MGR::S_ReqPhotoData, ofToString(photoID));
		return false;
	}
	else
	{
		//TODO
		title = Iter_->second.titleZH;
		msg = Iter_->second.msgZH;
		return true;
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
void dataHolder::setPhotoHeader(Json::Value & root)
{
	int size_ = root.size();
	for (int idx_ = 0; idx_ < size_; idx_++)
	{
		stPhotoHeader header_;
		string fileName_ = root[idx_].get("fileName", "").asString();
		header_.id = stoi(root[idx_].get("pid", "0").asString());
		header_.shape = (ePhotoShape)stoi(root[idx_].get("shape", "0").asString());
		header_.type = stoi(root[idx_].get("tid", "0").asString());
		header_.category = (ePhotoPrimaryCategory)stoi(root[idx_].get("cid", "0").asString());
		header_.thumbnailPath = configMgr::exPhotoPath + configMgr::exThumbFolderName + fileName_ + cPhotoExt;
		header_.sourcePath = configMgr::exPhotoPath + configMgr::exSourceFolderName + fileName_ + cPhotoExt;


		addPhotoMap(header_);
	}

	_headerSetup = true;
}

//--------------------------------------------------------------
void dataHolder::setPhotoHeaderData(int photoID, Json::Value & root)
{
	auto photoHeader_ = getPhotoHeader(photoID);
	photoHeader_.titleEN = root[0].get("enTitle", "").asString();
	photoHeader_.titleZH = root[0].get("zhTitle", "").asString();
	photoHeader_.msgEN = root[0].get("enMsg", "").asString();
	photoHeader_.msgZH = root[0].get("zhMsg", "").asString();
	setPhotoMap(photoHeader_);
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

//--------------------------------------------------------------
void dataHolder::setPhotoMap(stPhotoHeader & newPhotoHeader)
{
	if (_photoMap.find(newPhotoHeader.id) != _photoMap.end())
	{
		_photoMap[newPhotoHeader.id] = newPhotoHeader;
	}
}


#pragma endregion

#pragma region Server
//--------------------------------------------------------------
void dataHolder::setupServer()
{
	ofAddListener(_server.newResponseEvent, this, &dataHolder::onServerResponse);
	_server.start();
	
}

//--------------------------------------------------------------
void dataHolder::postToServer(string active, string param)
{
	ofxHttpForm form_;
	form_.action = configMgr::exServerUrl;
	form_.method = OFX_HTTP_POST;
	form_.addFormField("active", active);
	form_.addFormField("cmd", param);
	_server.addForm(form_);
}

//--------------------------------------------------------------
void dataHolder::handleActive(string active, Json::Value& root)
{
	Json::Reader reader_;
	if (active == NAME_MGR::S_ReqInitData)
	{
		setPhotoCategoryName(root.get("CategoryList", 0));
		setPhotoTypeName(root.get("TypeList", 0));

	}
	else if (active == NAME_MGR::S_ReqPhotoList)
	{
		setPhotoHeader(root.get("photoList", 0));
	}
	else if (active == NAME_MGR::S_ReqPhotoData)
	{
		int photoID_ = stoi(root.get("photoID", 0).asString());
		setPhotoHeaderData(photoID_, root.get("photoData", 0));
		ofNotifyEvent(_onPhotoDataLoad, photoID_);
	}
	else
	{
		ofLog(OF_LOG_WARNING, "[dataHolder::handleActive]Unknow active :" + active);
	}

	//Check Setup Finish
	if ((active == NAME_MGR::S_ReqInitData || active == NAME_MGR::S_ReqPhotoList) && setupCheck())
	{
		ofNotifyEvent(_onSetupFinish);
	}
}

//--------------------------------------------------------------
void dataHolder::onServerResponse(ofxHttpResponse & e)
{
	if (e.status != 200)
	{
		ofLog(OF_LOG_WARNING, "[dataHolder::onServerResponse] Http error :" + e.reasonForStatus);
		return;
	}

	Json::Value root_;
	Json::Reader reader_;

	if (!reader_.parse(e.responseBody.getText(), root_))
	{
		ofLog(OF_LOG_WARNING, "[dataHolder::onServerResponse] Decode json failed");
		return;
	}

	string result_ = root_.get("result", 0).asString();
	if (result_ != "1")
	{
		ofLog(OF_LOG_WARNING, "[dataHolder::onServerResponse] Http request failed");
		return;
	}
	string active_ = root_.get("active", 0).asString();
	handleActive(active_, root_);
	
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
