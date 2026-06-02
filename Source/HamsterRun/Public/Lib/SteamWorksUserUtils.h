// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Steam/steam_api.h"

#include "SteamWorksUserUtils.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERRUN_API USteamWorksUserUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	explicit  USteamWorksUserUtils();
	
	UFUNCTION(BlueprintCallable, Category = "SteamWorks")
	static bool ProfileColor(UTexture2D* avatar, FLinearColor& outColor);

private:
	static UTexture2D* RenderToTexture(int Image);
};
