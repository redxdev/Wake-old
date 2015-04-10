#include "Asset.h"

CLOG_LOGGER_DEF(AssetManager);

AssetManager& AssetManager::Get()
{
	static AssetManager Instance;
	return Instance;
}

AssetManager::AssetManager()
{
	
}

AssetManager::~AssetManager()
{
	
}


bool AssetManager::Startup()
{
	CLOG_DEBUG("Asset manager startup");

	return true;
}

bool AssetManager::Shutdown()
{
	CLOG_DEBUG("Asset manager shutdown");

	return true;
}

void AssetManager::Clean()
{
	
}