#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../Logging/LogMacros.h"

#define W_ASSET (AssetManager::Get())

class Asset
{
public:
	Asset(const std::string& Source);
	virtual ~Asset();

	const std::string& GetSource() const;

	virtual void Load() = 0;
	virtual void Unload() = 0;

private:
	std::string Source;
};

typedef std::shared_ptr<Asset> AssetPtr;

class AssetManager
{
	CLOG_LOGGER_DECL;
public:
	static AssetManager& Get();

	bool Startup();

	bool Shutdown();

private:
	AssetManager();
	~AssetManager();

	std::unordered_map<std::string, AssetPtr> Assets;
};