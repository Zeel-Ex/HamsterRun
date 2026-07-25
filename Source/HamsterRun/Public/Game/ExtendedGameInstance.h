// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "AdvancedFriendsGameInstance.h"
#include "Lib/SteamWorksUserUtils.h"

#include "ExtendedGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnForwardLocalPlayerEvent, ULocalPlayer*, LocalPlayer, FPlatformUserId, UserId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGamepadConnectionChangedEvent, EInputDeviceConnectionState, NewState, FPlatformUserId, UserId, FInputDeviceId, DeviceId);

/**
 * 
 */
UCLASS(Blueprintable)
class HAMSTERRUN_API UExtendedGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UPROPERTY(BlueprintAssignable)
	FOnForwardLocalPlayerEvent OnForwardLocalPlayerAddedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnForwardLocalPlayerEvent OnForwardLocalPlayerRemovedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGamepadConnectionChangedEvent OnGamepadConnectionChangedEvent;
	
	TMap<FInputDeviceId, FPlatformUserId> GetUserToDeviceMap() const { return UserToDeviceMap; }
	
	UFUNCTION(BlueprintCallable)
	TArray<FPlatformUserId> GetLocalPlayerIds()
	{
		TArray<FPlatformUserId> Ids;
		
		for (auto It = UserToDeviceMap.CreateIterator(); It; ++It)
		{
			Ids.Add(It.Value());
		}
		
		return Ids;
	};
	//vars
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USteamWorksUserUtils> steamWorksUserUtils;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static UExtendedGameInstance* GetExtendedGameInstance(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	int RemoveAdditionalLocalPlayers();
	
	static bool UserHardwareIsGamepad(FInputDeviceId DeviceId);
	
	void OnGamepadConnectionChanged(EInputDeviceConnectionState NewState, FPlatformUserId UserId,
	                                FInputDeviceId DeviceId);
	
	FTimerHandle GamepadRecheckTimerHandle;
	
	void CheckGamepadHardware(EInputDeviceConnectionState NewState, const FPlatformUserId UserId,
														   const FInputDeviceId DeviceId);

private:
	TMap<FInputDeviceId, FPlatformUserId> UserToDeviceMap;
};
