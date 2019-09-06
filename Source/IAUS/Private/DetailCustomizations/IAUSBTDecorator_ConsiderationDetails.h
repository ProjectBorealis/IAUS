#pragma once

#if WITH_EDITOR
#include "CoreMinimal.h"
#include "Editor/PropertyEditor/Public/IDetailCustomization.h"

class FIAUSBTDecorator_ConsiderationDetails : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();

	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	TSharedPtr<class SResponseCurvePreviewWidget> PreviewWidget;

};
#endif
