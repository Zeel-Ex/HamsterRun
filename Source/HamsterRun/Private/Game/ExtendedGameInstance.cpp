// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ExtendedGameInstance.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GameMapsSettings.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Interfaces/InputConnection.h"

void UExtendedGameInstance::Init()
{
	Super::Init();

	// init
	steamWorksUserUtils = NewObject<USteamWorksUserUtils>(this);
	
	OnLocalPlayerAddedEvent.AddLambda([this](ULocalPlayer* NewLocalPlayer)
	{
		OnForwardLocalPlayerAddedEvent.Broadcast(NewLocalPlayer, NewLocalPlayer->GetPlatformUserId());
	});
	
	OnLocalPlayerRemovedEvent.AddLambda([this](ULocalPlayer* OldLocalPlayer)
	{
		OnForwardLocalPlayerRemovedEvent.Broadcast(OldLocalPlayer, OldLocalPlayer->GetPlatformUserId());
	});
	
	const IPlatformInputDeviceMapper& PlatformInputDeviceMapper = IPlatformInputDeviceMapper::Get();

	PlatformInputDeviceMapper.GetOnInputDeviceConnectionChange()
							 .AddUObject(this, &UExtendedGameInstance::OnGamepadConnectionChanged);
	
	TArray<FPlatformUserId> users;
	PlatformInputDeviceMapper.GetAllActiveUsers(users);
	UE_LOG(LogTemp, Warning, TEXT("Users: %i"), users.Num());
	for (FPlatformUserId user : users)
	{
		if (UserHardwareIsGamepad(user))
		{
			UserToDeviceMap.Add(UInputDeviceSubsystem::Get()->GetMostRecentlyUsedInputDeviceId(user), user);
			UE_LOG(LogTemp, Warning, TEXT("Game pad: %i"), user.GetInternalId());
		}
	}
	
	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

void UExtendedGameInstance::Shutdown()
{
	Super::Shutdown();
}

UExtendedGameInstance* UExtendedGameInstance::GetExtendedGameInstance(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->
		GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			return Cast<UExtendedGameInstance>(GameInstance);
		}
	}
	return nullptr;
}

bool UExtendedGameInstance::UserHardwareIsGamepad(FPlatformUserId UserId)
{
	UInputDeviceSubsystem* ids = UInputDeviceSubsystem::Get();
		
	FHardwareDeviceIdentifier ident = ids->GetMostRecentlyUsedHardwareDevice(UserId);
	return (ident.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad);
}

void UExtendedGameInstance::OnGamepadConnectionChanged(EInputDeviceConnectionState NewState, FPlatformUserId UserId,
														   FInputDeviceId DeviceId)
{
	switch (NewState)
	{
	case EInputDeviceConnectionState::Invalid:
		[[fallthrough]];
	case EInputDeviceConnectionState::Unknown:
		[[fallthrough]];
	case EInputDeviceConnectionState::Disconnected:
		{
			if ( UserToDeviceMap.Remove(DeviceId) > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("OnGamepadConnectionChanged: %i, %i, %i"), (int)NewState, UserId.GetInternalId(), DeviceId.GetId());
				OnGamepadConnectionChangedEvent.Broadcast(NewState, UserId, DeviceId);
			}
			
			break;
		}
	case EInputDeviceConnectionState::Connected:
		if (UserHardwareIsGamepad(UserId))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnGamepadConnectionChanged: %i, %i, %i"), (int)NewState, UserId.GetInternalId(), DeviceId.GetId());
			UserToDeviceMap.Add(DeviceId, UserId);
			OnGamepadConnectionChangedEvent.Broadcast(NewState, UserId, DeviceId);
		}
		break;
	}
}
