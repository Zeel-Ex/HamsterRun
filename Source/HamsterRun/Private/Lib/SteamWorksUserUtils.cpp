// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SteamWorksUserUtils.h"
#include "Engine/Texture2D.h"

USteamWorksUserUtils::USteamWorksUserUtils()
{
}

bool USteamWorksUserUtils::ProfileColor(UTexture2D* avatar, FLinearColor& outColor)
{
	if (!avatar)
	{
		return false;
	}

	FTexturePlatformData* PlatformData = avatar->GetPlatformData(); // UE5. In UE4 this is the raw member `PlatformData`
	if (!PlatformData || PlatformData->Mips.Num() == 0)
	{
		return false;
	}

	FTexture2DMipMap& Mip = PlatformData->Mips[0];
	const int32 Width  = Mip.SizeX;
	const int32 Height = Mip.SizeY;

	const void* RawData = Mip.BulkData.LockReadOnly();
	if (!RawData)
	{
		Mip.BulkData.Unlock();
		return false;
	}

	const FColor* PixelData = static_cast<const FColor*>(RawData);
	const int32 NumPixels = Width * Height;

	float BestSaturation = -1.0f;
	FLinearColor HighlightColor = FLinearColor::White;

	for (int32 i = 0; i < NumPixels; ++i)
	{
		const FColor& Pixel = PixelData[i];

		if (Pixel.A == 0) // skip transparent
		{
			continue;
		}

		const FLinearColor Linear(Pixel); // gamma-correct sRGB -> linear decode

		const float MaxC = FMath::Max3(Linear.R, Linear.G, Linear.B);
		const float MinC = FMath::Min3(Linear.R, Linear.G, Linear.B);

		if (MaxC < 0.15f || MinC > 0.9f) // skip near-black / near-white
		{
			continue;
		}

		const float Saturation = (MaxC <= 0.0f) ? 0.0f : (MaxC - MinC) / MaxC;

		if (Saturation > BestSaturation)
		{
			BestSaturation = Saturation;
			HighlightColor = Linear;
		}
	}

	Mip.BulkData.Unlock();

	outColor = HighlightColor;
	return true;
}

UTexture2D* USteamWorksUserUtils::RenderToTexture(int Image)
{
	uint32 Width = 0, Height = 0;
	if (!SteamUtils()->GetImageSize(Image, &Width, &Height))
		return nullptr;

	TArray<uint8> RawData;
	RawData.SetNumUninitialized(Width * Height * 4);

	if (!SteamUtils()->GetImageRGBA(Image, RawData.GetData(), RawData.Num()))
		return nullptr;

	// 🔹 Create texture (same as before)
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);

	void* Data = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, RawData.GetData(), RawData.Num());
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	Texture->UpdateResource();
	return Texture;
}