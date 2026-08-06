// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/WidgetInteractionLib.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

UUserWidget* UWidgetInteractionLib::GetNearestWidget(TArray<UUserWidget*> Widgets, const FVector2D& InteractionLocation,
                                                     float Threshold)
{
	UUserWidget* NearestWidget = nullptr;

	for (UUserWidget* Widget : Widgets)
	{
		if (!Widget)
			continue;
		
		FGeometry geom = Widget->GetCachedGeometry();
		
		FVector2D WidgetPosition = geom.GetAbsolutePosition();
		WidgetPosition += geom.GetLocalSize() * 0.5f;
		
		FVector2D PixelPosition;
		FVector2D ViewportPosition;
    
		// USlateBlueprintLibrary::AbsoluteToViewport handles the DPI math for you
		USlateBlueprintLibrary::AbsoluteToViewport(
			Widget, 
			WidgetPosition, 
			PixelPosition, 
			ViewportPosition
		);
		
		float distance = FVector2D::Distance(ViewportPosition, InteractionLocation);
		if (distance <= Threshold + geom.GetLocalSize().X)
		{
			NearestWidget = Widget;
			break;
		}
	}

	return NearestWidget;
}

void UWidgetInteractionLib::RemoveAllWidgetsFromController(APlayerController* PlayerController, TSubclassOf<UUserWidget> WidgetClass, UObject* WorldContextObject)
{
	TArray<UUserWidget*> Widgets;
	
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WorldContextObject, Widgets, WidgetClass, false);
	
	for (auto It = Widgets.CreateIterator(); It; ++It)
	{
		if (!(*It))
			return;
		
		if (PlayerController == (*It)->GetOwningPlayer())
		{
			(*It)->RemoveFromParent();
			It.RemoveCurrent();
		}
	}
}
