// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RemoteMovementBase.generated.h"

UENUM(Blueprintable)
enum CurrentNetMode : int
{
	Standalone,
	DedicatedServer,
	ListenServer,
	Client
};

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAMSTERRUN_API URemoteMovementBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URemoteMovementBase();
	
	UFUNCTION(BlueprintPure, Category = "Networking")
	CurrentNetMode GetCurrentNetMode() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
