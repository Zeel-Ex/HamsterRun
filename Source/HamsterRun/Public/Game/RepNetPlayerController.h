// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RepNetPlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class HAMSTERRUN_API ARepNetPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerStateChanged(APlayerState* NewPlayerState);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerReceived();
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChangedDelegate;
	
        virtual void PostSeamlessTravel() override;
    
        UFUNCTION(BlueprintImplementableEvent, Category = "Seamless Travel")
        void OnPostSeamlessTravel();
};
