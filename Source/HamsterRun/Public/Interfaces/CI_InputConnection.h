// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CI_InputConnection.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCI_InputConnection : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HAMSTERRUN_API ICI_InputConnection
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(Blueprintable, BlueprintNativeEvent)
	void OnGamepadConnected(FPlatformUserId UserId, FInputDeviceId DeviceId);
	
	UFUNCTION(Blueprintable, BlueprintNativeEvent)
	void OnGamepadDisconnected(FPlatformUserId UserId, FInputDeviceId DeviceId);
};
