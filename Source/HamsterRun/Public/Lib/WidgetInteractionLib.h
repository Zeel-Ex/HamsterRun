// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetInteractionLib.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERRUN_API UWidgetInteractionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HamsterRun|WidgetInteractionLib")
	static UUserWidget* GetNearestWidget(TArray<UUserWidget*> Widgets, const FVector2D& InteractionLocation,
	                                     float Threshold);

	UFUNCTION(BlueprintCallable, Category = "HamsterRun|WidgetInteractionLib", meta=(WorldContext="WorldContextObject"))
	static void RemoveAllWidgetsFromController(APlayerController* PlayerController, TSubclassOf<UUserWidget> WidgetClass, UObject* WorldContextObject);
};
