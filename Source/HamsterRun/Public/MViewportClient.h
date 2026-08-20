// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UINavGameViewportClient.h"
#include "MViewportClient.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HAMSTERRUN_API UMViewportClient : public UUINavGameViewportClient
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Splitscreen")
	void EnableListenForInput(bool bEnable);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static UMViewportClient* GetMyGameViewportClient(const UObject* WorldContextObject);

	FOnLocalPlayerEvent OnLocalPlayerAddedEvent;

private:
	bool bListenForLocalPlayerInput = false;
	
	FOverrideInputKeyHandler CachedInputKeyHandler;
	
	bool OnAnyInputKeyEvent(FInputKeyEventArgs& InputKeyEventArgs) const;
};
