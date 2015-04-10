#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../Logging/LogMacros.h"

#define W_ASSET (AssetManager::Get())

class AssetManager
{
	CLOG_LOGGER_DECL;
public:
	static AssetManager& Get();

	bool Startup();

	bool Shutdown();

	void Clean();

private:
	AssetManager();
	~AssetManager();
};