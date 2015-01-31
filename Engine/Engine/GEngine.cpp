#include "GEngine.h"

namespace Engine
{
	CLOG_LOGGER_DEF(Engine::GEngine);

	GEngine& GEngine::Get()
	{
		static GEngine Instance;
		return Instance;
	}

	GEngine::GEngine()
	{
	}

	GEngine::~GEngine()
	{
	}

	bool GEngine::Startup()
	{
		CLOG_INFO("Engine startup");

		auto BaseEnt = W_ENTITY.CreateEntity<Entity>();
		if (!BaseEnt)
		{
			CLOG_FATAL("Unable to reserve base EID from entity manager");
			return false;
		}

		BaseEID = BaseEnt->GetEntityId();
		return true;
	}

	bool GEngine::Shutdown()
	{
		CLOG_INFO("Engine shutdown");
		return true;
	}
}