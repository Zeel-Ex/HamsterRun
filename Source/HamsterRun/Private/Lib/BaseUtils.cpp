// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/BaseUtils.h"

#include "Engine/TextureRenderTarget2D.h"

UTexture2D* UBaseUtils::GetTextureRenderTarget(UTextureRenderTarget2D* RTarget)
{
	if (!RTarget || !RTarget->IsValidLowLevel()) return nullptr;

	UTexture2D* Texture2D = UTexture2D::CreateTransient(
		RTarget->SizeX,
		RTarget->SizeY,
		RTarget->GetFormat()
	);

	if (!Texture2D) return nullptr;

	// Read pixels from the render target
	TArray<FColor> Pixels;
	FRenderTarget* RenderTarget = RTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(Pixels);

	// Write into the texture
	FTexture2DMipMap& Mip = Texture2D->GetPlatformData()->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
	Mip.BulkData.Unlock();

	Texture2D->SRGB = 1;
	Texture2D->UpdateResource();

	return Texture2D;
}

FString UBaseUtils::GetAppVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}

bool UBaseUtils::GetActorScreenBounds(APlayerController* PC, AActor* Actor, FBox2D& OutScreenBox)
{
	if (!PC || !Actor) return false;
    
	FVector Origin, Extent;
	Actor->GetActorBounds(false, Origin, Extent);
    
	OutScreenBox = FBox2D(ForceInit);
	bool bAnyOnScreen = false;
    
	for (int32 x = -1; x <= 1; x += 2)
		for (int32 y = -1; y <= 1; y += 2)
			for (int32 z = -1; z <= 1; z += 2)
			{
				FVector Corner = Origin + FVector(x * Extent.X, y * Extent.Y, z * Extent.Z);
				FVector2D ScreenPos;
				if (PC->ProjectWorldLocationToScreen(Corner, ScreenPos))
				{
					OutScreenBox += ScreenPos;
					bAnyOnScreen = true;
				}
			}
    
	return bAnyOnScreen;
}
