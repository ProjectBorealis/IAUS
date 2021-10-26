// Copyright Project Borealis

#include "IAUSModule.h"

#include "DetailCustomizations/IAUSBTDecorator_ConsiderationDetails.h"
#include "IAUS/IAUSCommon.h"

#if WITH_EDITOR
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#endif

#define LOCTEXT_NAMESPACE "FIAUSModule"

// Log categories
DEFINE_LOG_CATEGORY(LogIAUS);

void FIAUSModule::StartupModule()
{
#if WITH_EDITOR
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("IAUSBTDecorator_Consideration",
											 FOnGetDetailCustomizationInstance::CreateStatic(&FIAUSBTDecorator_ConsiderationDetails::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
#endif
}

void FIAUSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FIAUSModule, IAUS)
