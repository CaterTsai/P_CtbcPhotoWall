#pragma once

#include "ofxHttpUtils.h"
#include "json.h"

#include "constParameter.h"
#include "photoRender.h"
#include "configMgr.h"
#include "textUnit.h"


class dataHolder
{
public:
	void setup();
	void update(float delta);
public:
	bool setupCheck();

#pragma region Photo Category
public:
	string getCategoryName(ePhotoPrimaryCategory eCategory, bool isZH);
	string getCategorySubtitle(ePhotoPrimaryCategory eCategory, bool isZH);

private:
	void setPhotoCategory(Json::Value& root);
private:
	bool _categorySetup;
	map<ePhotoPrimaryCategory, textUnit>	_categoryName;
	map<ePhotoPrimaryCategory, textUnit>	_categorySubtitle;
#pragma endregion

#pragma region Photo Type
public:
	vector<PHOTO_TYPE>	getType(ePhotoPrimaryCategory eCategory);
	string getTypeName(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type, bool isZH);
	PHOTO_TYPE getSmileType();

private:
	void setPhotoTypeName(Json::Value& root);

private:
	bool _typeSetup;
	map<ePhotoPrimaryCategory, map<PHOTO_TYPE, textUnit>>	_photoTypeName;
#pragma endregion

#pragma region Smile Photo
private:
	void loadSmilePhoto();
	void checkSmileFile(float delta);
	void addSmileHeader(string order);

private:
	int _smileBaseID;
	PHOTO_TYPE _smileType;
	float _timer;

public:
	ofEvent<stPhotoHeader> _onNewSmilePhoto;
#pragma endregion
	
#pragma region PhotoHeader
public:
	stPhotoHeader& getPhotoHeader(int photoId);
	bool getPhotoText(int photoID, bool isZH, string& title, string& msg);

	vector<int> getPhotoID(ePhotoPrimaryCategory eCategory);	
	vector<int> getPhotoID(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type);
	vector<int> getPhotoIDWithoutType(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type);
private:	
	void setPhotoHeader(Json::Value& root);
	void setPhotoHeaderData(int photoID, Json::Value& root);
	void addPhotoMap(stPhotoHeader& photoHeader);
	void addIndex(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type, int photoid);
	void setPhotoMap(stPhotoHeader& newPhotoHeader);
private:
	bool _headerSetup;
	map<int, stPhotoHeader> _photoMap;
	map<PHOTO_TYPE, vector<int>> _typeToPhotoID[ePhotoCategory_Num];

#pragma endregion
	
#pragma region Server
private:
	void setupServer();
	void postToServer(string active, string param = "");
	void handleActive(string active, Json::Value& root);

public:
	void onServerResponse(ofxHttpResponse& e);
	ofEvent<void> _onSetupFinish;
	ofEvent<int> _onPhotoDataLoad;
private:
	ofxHttpUtils	_server;
#pragma endregion
	
#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	dataHolder()
		:_headerSetup(false)
		,_categorySetup(false)
		,_typeSetup(false)
		, _smileType(2)
	{};
	~dataHolder() {
		_server.stop();
	}
	dataHolder(dataHolder const&) {};
	void operator=(dataHolder const&) {};
public:
	static dataHolder* GetInstance();
	static void Destroy();

private:
	static dataHolder *pInstance;
#pragma endregion
};