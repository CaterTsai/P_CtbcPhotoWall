#pragma once

#include "constParameter.h"
#include "photoRender.h"
#include "textUnit.h"

class dataHolder
{
public:
	void setup(string url);
	
private:
	void setupCheck();

#pragma region Photo Category
public:
	string getCategoryName(ePhotoPrimaryCategory eCategory, bool isZH);
	textUnit getCategoryName(ePhotoPrimaryCategory eCategory);

private:
	void loadPhotoCategoryName();
private:
	map<ePhotoPrimaryCategory, textUnit>	_categoryName;
#pragma endregion

#pragma region Photo Type
public:
	vector<PHOTO_TYPE>	getType(ePhotoPrimaryCategory eCategory);
	string getTypeName(ePhotoPrimaryCategory eCategory, PHOTO_TYPE type, bool isZH);
	
private:
	void loadPhotoTypeName();

private:
	map<ePhotoPrimaryCategory, map<PHOTO_TYPE, textUnit>>	_photoTypeName;
#pragma endregion

#pragma region PhotoHeader
public:
	stPhotoHeader& getPhotoHeader(int photoId);
	stPhotoData getPhotoData(int photoId);
	vector<int> getPhotoID(PHOTO_TYPE type);
	vector<int> getPhotoID(ePhotoPrimaryCategory eCategory);

private:
	
	void loadPhotoHeader();

	void addPhotoMap(stPhotoHeader& photoHeader);
	void addType2PhotoID(PHOTO_TYPE type, int photoid);
	void addCategory2PhotoID(ePhotoPrimaryCategory, int photoid);

private:
	bool _isSetup;
	string _backendUrl;
	map<int, stPhotoHeader> _photoMap;
	map<PHOTO_TYPE, vector<int>> _typeToPhotoID;
	map<ePhotoPrimaryCategory, vector<int>>	_categoryToPhotoID;
#pragma endregion
		
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