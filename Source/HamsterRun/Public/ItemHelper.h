// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/OverlapResult.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemHelper.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERRUN_API UItemHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "ItemHelper/LeafBlower", meta = (WorldContext = "WorldContextObject"))
	static void ApplyBlowerForce(const FTransform& BlowerTransform, float MaxRange, float ConeHalfAngleDegrees, float BaseForceStrength, UObject* WorldContextObject);
};
