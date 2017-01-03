#pragma once

#include "constParameter.h"
#include "renderMgr.h"

class dataHolder
{
public:
	void setup(string url);
	
	stPhotoHeader& getPhotoHeader(int photoId);
	stPhotoData getPhotoData(int photoId);
	vector<int> getPhotoID(PHOTO_TYPE type);
	vector<int> getPhotoID(ePhotoPrimaryCategory eCategory);

private:
	void setupCheck();
	void loadPhotoHeader();
	

private:
	bool _isSetup;
	string _backendUrl;
	map<int, stPhotoHeader> _photoMap;
	map<PHOTO_TYPE, vector<int>> _typeToPhotoID;
	map<ePhotoPrimaryCategory, vector<int>>	_categoryToPhotoID;
	
#pragma region Singleton
	//-------------------
	//Singleton
	//-------------------
private:
	dataHolder()
		:_isSetup(false)
		,_backendUrl("")
	{};
	~dataHolder() {
		Destroy();
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