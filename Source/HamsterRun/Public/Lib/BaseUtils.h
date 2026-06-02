// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseUtils.generated.h"

/**
 * 
 */
UCLASS()
class HAMSTERRUN_API UBaseUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UTexture2D* GetTextureRenderTarget(UTextureRenderTarget2D* RTarget);
};
