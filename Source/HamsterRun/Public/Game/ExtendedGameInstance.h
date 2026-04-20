// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"

#include "ExtendedGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class HAMSTERRUN_API UExtendedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	TMap<FPlatformUserId, FInputDeviceId> GetUserToDeviceMap() const { return UserToDeviceMap; }
	
	UFUNCTION(BlueprintCallable)
	TArray<FPlatformUserId> GetLocalPlayerIds() const
	{
		TArray<FPlatformUserId> keys;
		UserToDeviceMap.GetKeys(keys);
		return keys;
	};
	
private:
	TMap<FPlatformUserId, FInputDeviceId> UserToDeviceMap;

	void OnInputDeviceConnectionChanged(
		EInputDeviceConnectionState NewState,
		FPlatformUserId          UserId,
		FInputDeviceId           DeviceId);
	
	static bool UserHardwareIsGamepad(FPlatformUserId UserId);
};
