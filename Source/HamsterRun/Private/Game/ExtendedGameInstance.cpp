// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ExtendedGameInstance.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GameMapsSettings.h"
#include "InterchangeResult.h"
#include "MoviePlayer.h"
#include "SNegativeActionButton.h"
#include "SWarningOrErrorBox.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Interfaces/InputConnection.h"
#include "VerseVM/VVMRuntimeError.h"
#include "SCharacterProfileWidget.h"

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
	
	TArray<FInputDeviceId> InputDevices;
	PlatformInputDeviceMapper.GetAllConnectedInputDevices(InputDevices);
	UE_LOG(LogTemp, Warning, TEXT("Input Devices: %i"), InputDevices.Num());
	for (const FInputDeviceId& Device : InputDevices)
	{
		if (UserHardwareIsGamepad(Device))
		{
			UserToDeviceMap.Add(Device, PlatformInputDeviceMapper.GetUserForInputDevice(Device));
		}
	}
	
	// Loading Screen
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UExtendedGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UExtendedGameInstance::OnPostLoadMap);
	FWorldDelegates::OnSeamlessTravelStart.AddUObject(this, &UExtendedGameInstance::OnSeamlessTravelStart);

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

int UExtendedGameInstance::RemoveAdditionalLocalPlayers()
{
	int RemovedCount = 0;
	
	if (LocalPlayers.Num() > 1)
		{
			for (int i = LocalPlayers.Num() - 1; i > 0; --i)
			{
				if (ULocalPlayer* PlayerToRemove = LocalPlayers[i]; PlayerToRemove != GetFirstGamePlayer())
				{
					RemoveLocalPlayer(PlayerToRemove);
					++RemovedCount;
				}
			}
		}
	
	return RemovedCount;
}

bool UExtendedGameInstance::UserHardwareIsGamepad(FInputDeviceId DeviceId)
{
	UInputDeviceSubsystem* Ids = UInputDeviceSubsystem::Get();
		
	FHardwareDeviceIdentifier ident = Ids->GetInputDeviceHardwareIdentifier(DeviceId);
	
	UE_LOG(LogTemp, Warning, TEXT("UserHardwareGamepadType: %i"), ident.PrimaryDeviceType);
	
	return (ident.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad);
}

void UExtendedGameInstance::OnGamepadConnectionChanged(EInputDeviceConnectionState NewState, const FPlatformUserId UserId,
														   const FInputDeviceId DeviceId)
{
	GetTimerManager().SetTimer(GamepadRecheckTimerHandle, [this, DeviceId, UserId, NewState]()
{
	CheckGamepadHardware(NewState, UserId, DeviceId);
}, 1.0f, false);
	
}

void UExtendedGameInstance::CheckGamepadHardware(EInputDeviceConnectionState NewState, const FPlatformUserId UserId,
														   const FInputDeviceId DeviceId)
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
		{
			if (UserHardwareIsGamepad(DeviceId))
			{
				UE_LOG(LogTemp, Warning, TEXT("OnGamepadConnectionChanged: %i, %i, %i"), (int)NewState, UserId.GetInternalId(), DeviceId.GetId());
				UserToDeviceMap.Add(DeviceId, UserId);
				OnGamepadConnectionChangedEvent.Broadcast(NewState, UserId, DeviceId);
			}
			break;
		}
	}
}

void UExtendedGameInstance::OnPreLoadMap(const FString& MapName)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPreLoadMap"));
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreenWidget = SNew(SCharacterProfileWidget);
		LoadingScreen.WidgetLoadingScreen = LoadingScreenWidget;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 2.0f;
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UExtendedGameInstance::OnPostLoadMap(UWorld* LoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPostLoadMap: %s"), *LoadedWorld->GetName());
	if (!IsRunningDedicatedServer())
	{
		GetMoviePlayer()->StopMovie();
		GEngine->GameViewport->RemoveViewportWidgetContent(LoadingScreenWidget.ToSharedRef());
	}
}

void UExtendedGameInstance::OnSeamlessTravelStart(UWorld* CurrentWorld, const FString& LevelName)
{
	if (IsRunningDedicatedServer())
		return;
	
	LoadingScreenWidget = SNew(SCharacterProfileWidget);
	GEngine->GameViewport->AddViewportWidgetContent(LoadingScreenWidget.ToSharedRef(), 1000);
}
