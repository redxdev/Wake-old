#include "Asset.h"

CLOG_LOGGER_DEF(AssetManager);

AssetManager& AssetManager::Get()
{
	static AssetManager Instance;
	return Instance;
}

bool AssetManager::Startup()
{
	CLOG_DEBUG("Asset manager startup");

	return true;
}

bool AssetManager::Shutdown()
{
	CLOG_DEBUG("Asset manager shutdown");

	for (auto Itr = Assets.begin(); Itr != Assets.end(); ++Itr)
	{
		Itr->second->Unload();
	}

	Assets.clear();

	CLOG_DEBUG("Unloaded " << Assets.size() << " assets for shutdown");

	for (auto Itr = Loaders.begin(); Itr != Loaders.end(); ++Itr)
	{
		delete *Itr;
	}

	return true;
}

void AssetManager::Clean()
{
	uint32 count = 0;
	auto Itr = Assets.begin();
	while (Itr != Assets.end())
	{
		if (Itr->second.unique())
		{
			AssetPtr Asset = Itr->second;
			Itr = Assets.erase(Itr);

			Asset->Unload();
			++count;
		}
		else
		{
			++Itr;
		}
	}

	if (count > 0)
	{
		CLOG_DEBUG("Unloaded " << count << " assets");
	}
}