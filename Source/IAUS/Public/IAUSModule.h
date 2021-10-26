// Copyright Project Borealis

#pragma once

#include "Modules/ModuleManager.h"

class FIAUSModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
