// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/RemoteMovementBase.h"

// Sets default values for this component's properties
URemoteMovementBase::URemoteMovementBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void URemoteMovementBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void URemoteMovementBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

CurrentNetMode URemoteMovementBase::GetCurrentNetMode(const UObject* WorldContextObject)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		return static_cast<CurrentNetMode>(World->GetNetMode());
	}
	return Standalone;
}
