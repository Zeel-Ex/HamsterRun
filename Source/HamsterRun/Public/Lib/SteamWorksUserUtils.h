// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Steam/steam_api.h"

#include "SteamWorksUserUtils.generated.h"

UDELEGATE(BlueprintCosmetic)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAvatarReady, const UTexture2D*, avatar, const FString&, SteamID64);
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
	static UTexture2D* GetSteamAvatar(const FString& SteamID64);
	
	UFUNCTION(BlueprintCallable, Category = "SteamWorks")
	static bool ProfileColor(UTexture2D* avatar, FLinearColor& outColor);

	STEAM_CALLBACK(USteamWorksUserUtils, OnAvatarLoaded, AvatarImageLoaded_t);
	
	UPROPERTY(BlueprintAssignable)
	FAvatarReady OnAvatarLoadedDelegate;
	
private:
	static UTexture2D* RenderToTexture(int Image);
};
