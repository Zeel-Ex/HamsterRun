// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ExtendedGameInstance.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GameMapsSettings.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Interfaces/CI_InputConnection.h"

bool UExtendedGameInstance::userHardwareIsGamepad(FPlatformUserId UserId)
{
	UInputDeviceSubsystem* ids = UInputDeviceSubsystem::Get();
		
	FHardwareDeviceIdentifier ident = ids->GetMostRecentlyUsedHardwareDevice(UserId);
	return (ident.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad);
}

void UExtendedGameInstance::Init()
{

	Super::Init();

	IPlatformInputDeviceMapper& platformInputDeviceMapper = IPlatformInputDeviceMapper::Get();

	platformInputDeviceMapper.GetOnInputDeviceConnectionChange()
	                         .AddUObject(this, &UExtendedGameInstance::OnInputDeviceConnectionChanged);
	
	TArray<FPlatformUserId> users;
	platformInputDeviceMapper.GetAllActiveUsers(users);
	UE_LOG(LogTemp, Warning, TEXT("Users: %i"), users.Num());
	for (FPlatformUserId user : users)
	{
		if (userHardwareIsGamepad(user))
		{
			UserToDeviceMap.Add(user, UInputDeviceSubsystem::Get()->GetMostRecentlyUsedInputDeviceId(user));
			UE_LOG(LogTemp, Warning, TEXT("Game pad: %i"), user.GetInternalId());
		}
	}
}

void UExtendedGameInstance::Shutdown()
{
	IPlatformInputDeviceMapper::Get()
		.GetOnInputDeviceConnectionChange()
		.RemoveAll(this);

	Super::Shutdown();
}

void UExtendedGameInstance::OnInputDeviceConnectionChanged(EInputDeviceConnectionState NewState, FPlatformUserId UserId,
                                                           FInputDeviceId DeviceId)
{
	switch (NewState)
	{
	case EInputDeviceConnectionState::Connected:
		{
			if (userHardwareIsGamepad(UserId))
			{
				UE_LOG(LogTemp, Warning, TEXT("Player Connected"));
				AGameModeBase* gM = GetWorld()->GetAuthGameMode<AGameModeBase>();
				if (gM)
				{
					ICI_InputConnection::Execute_OnGamepadConnected(gM, UserId, DeviceId);
				}
			
				UserToDeviceMap.Add(UserId, DeviceId);
			}
			break;
		}

	case EInputDeviceConnectionState::Disconnected:
		{
			if (LocalPlayers.Num() > 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player Disconnected"));
				AGameModeBase* gM = GetWorld()->GetAuthGameMode<AGameModeBase>();
				if (gM)
				{
					if (const FPlatformUserId* correctedUserId = UserToDeviceMap.FindKey(DeviceId))
					{
						ICI_InputConnection::Execute_OnGamepadDisconnected(gM, *correctedUserId, DeviceId);
					}
				}
			}
			break;
		}

	default:
		break;
	}
}