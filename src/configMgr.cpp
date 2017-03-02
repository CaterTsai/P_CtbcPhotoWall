#include "configMgr.h"

string configMgr::exThumbFolderName = "";
string configMgr::exSourceFolderName = "";
string configMgr::exSmileOrderFolderName = "";
string configMgr::exPhotoPath = "";
string configMgr::exSmilePath = "";

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
}


