#include "Asset.h"

Asset::Asset(const std::string& Source)
{
	this->Source = Source;
}

Asset::~Asset()
{
	
}

const std::string& Asset::GetSource() const
{
	return Source;
}

CLOG_LOGGER_DEF(AssetManager);

AssetManager& AssetManager::Get()
{
	static AssetManager Instance;
	return Instance;
}

bool AssetManager::Startup()
{
	return true;
}

bool AssetManager::Shutdown()
{
	return true;
}
