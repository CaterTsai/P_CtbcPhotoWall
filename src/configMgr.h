#pragma once
#include "ofxXmlSettings.h"
#include "constParameter.h"

class configMgr
{
public:
	static void load(string path);
	
public:
	static string exThumbFolderName;
	static string exSourceFolderName;
	static string exSmileOrderFolderName;
	static string exPhotoPath;
	static string exSmilePath;
	static string exServerUrl;
	static float exIdleTime;
	static bool exConnectSmile;
};
