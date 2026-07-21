// Fill out your copyright notice in the Description page of Project Settings.


#include "MViewportClient.h"

void UMViewportClient::EnableListenForInput(bool bEnable)
{ 
	bListenForLocalPlayerInput = bEnable;
	UE_LOG(LogTemp, Warning, TEXT("EnableLostenfidejbfhhbd"));

	if (!bListenForLocalPlayerInput)
	{
		// Stop listening: restore the previous handler
		OnOverrideInputKey() = CachedInputKeyHandler;
		CachedInputKeyHandler.Unbind();
	}
	else
	{
		// Start listening: store the current handler so we can call it later
		CachedInputKeyHandler = OnOverrideInputKey();
        
		// Bind our custom function to intercept all key inputs
		OnOverrideInputKey().BindUObject(this, &ThisClass::OnAnyInputKeyEvent);
	}
}

UMViewportClient* UMViewportClient::GetMyGameViewportClient(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (auto viewport = GameInstance->GetGameViewportClient())
			{
				return Cast<UMViewportClient>(viewport);
			}
		}
	}
	return nullptr;
}

bool UMViewportClient::OnAnyInputKeyEvent(FInputKeyEventArgs& InputKeyEventArgs) const
{
	static const TArray<FKey> JoinKeys = { EKeys::Gamepad_FaceButton_Bottom };

	if (InputKeyEventArgs.Event != IE_Pressed || !JoinKeys.Contains(InputKeyEventArgs.Key))
	{
        return CachedInputKeyHandler.IsBound() ? CachedInputKeyHandler.Execute(InputKeyEventArgs) : false;
	}
	
	ULocalPlayer* ExistingPlayer = GEngine->GetLocalPlayerFromInputDevice(this, InputKeyEventArgs.InputDevice);

	if (ExistingPlayer == nullptr)
	{
		FPlatformUserId NewUserId = IPlatformInputDeviceMapper::Get().GetUserForInputDevice(InputKeyEventArgs.InputDevice);

		FString OutError;
		ULocalPlayer* NewPlayer = GameInstance->CreateLocalPlayer(NewUserId, OutError, true);
		if (!NewPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create local player: %s"), *OutError);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Created new local player %d"), NewUserId.GetInternalId());
	}
	else if (ExistingPlayer != GameInstance->GetFirstGamePlayer())
	{
		UE_LOG(LogTemp, Warning, TEXT("Removing local player %d"), ExistingPlayer->GetPlatformUserId().GetInternalId());
		GameInstance->RemoveLocalPlayer(ExistingPlayer);
	}

	return true;
}
