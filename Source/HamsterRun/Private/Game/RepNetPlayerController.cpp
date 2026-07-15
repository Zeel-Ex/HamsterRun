// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RepNetPlayerController.h"

void ARepNetPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	OnPlayerStateChanged(PlayerState);
	OnPlayerStateChangedDelegate.Broadcast(PlayerState);
}

void ARepNetPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	OnPostSeamlessTravel();
}
