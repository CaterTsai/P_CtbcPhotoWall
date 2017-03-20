#include "configMgr.h"

string configMgr::exThumbFolderName = "";
string configMgr::exSourceFolderName = "";
string configMgr::exSmileOrderFolderName = "";
string configMgr::exPhotoPath = "";
string configMgr::exSmilePath = "";
string configMgr::exServerUrl = "";
float configMgr::exIdleTime = 30.0f;
bool configMgr::exConnectSmile = false;
void configMgr::load(string path)
{
	ofxXmlSettings xml_;
	if (!xml_.load(path))
	{
		ofLog(OF_LOG_ERROR, "[configMgr::load]Load config file failed");
		return;
	}

	exThumbFolderName = xml_.getValue("thubmFloderName", "", 0);
	exSourceFolderName = xml_.getValue("sourceFolderName", "", 0);
	exSmileOrderFolderName = xml_.getValue("orderFolderName", "", 0);
	exPhotoPath = xml_.getValue("photoPath", "", 0);
	exSmilePath = xml_.getValue("smilePath", "", 0);
	exServerUrl = xml_.getValue("serverUrl", "", 0);
	exIdleTime = xml_.getValue("idleTime", 30.0f, 0);
	exConnectSmile = (xml_.getValue("connectSmile", 0, 0) == 1);
}


