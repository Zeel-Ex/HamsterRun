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
