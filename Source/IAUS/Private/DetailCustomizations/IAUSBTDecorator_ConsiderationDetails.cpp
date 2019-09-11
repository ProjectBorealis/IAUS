#pragma once

#if WITH_EDITOR
#include "DetailCustomizations/IAUSBTDecorator_ConsiderationDetails.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"
#include "DetailCustomizations/SResponseCurvePreviewWidget.h"

#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/DetailWidgetRow.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"

#define LOCTEXT_NAMESPACE "IAUSBTDecorator_ConsiderationDetails"

TSharedRef<IDetailCustomization> FIAUSBTDecorator_ConsiderationDetails::MakeInstance()
{
	return MakeShareable( new FIAUSBTDecorator_ConsiderationDetails );
}

void FIAUSBTDecorator_ConsiderationDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	TWeakObjectPtr<UIAUSBTDecorator_Consideration> Consideration;
	TArray<TWeakObjectPtr<UObject> > EditedObjects;
	DetailLayout.GetObjectsBeingCustomized(EditedObjects);
	for (int32 i = 0; i < EditedObjects.Num(); i++)
	{
		const UIAUSBTDecorator_Consideration* EditedTest = Cast<const UIAUSBTDecorator_Consideration>(EditedObjects[i].Get());
		if (EditedTest)
		{
			Consideration = MakeWeakObjectPtr(const_cast<UIAUSBTDecorator_Consideration*>(EditedTest));
			break;
		}
	}

	// scoring & filter function preview
	IDetailCategoryBuilder& PreviewCategory = DetailLayout.EditCategory("Preview");
	PreviewCategory.AddCustomRow(LOCTEXT("Preview", "Preview")).WholeRowWidget
		[
			SAssignNew(PreviewWidget, SResponseCurvePreviewWidget)
		];

	PreviewWidget->ResponseCurve = Consideration->ResponseCurve;
}

#undef LOCTEXT_NAMESPACE
#endif
