// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SteamWorksUserUtils.h"
#include "Engine/Texture2D.h"

USteamWorksUserUtils::USteamWorksUserUtils()
{
}

bool USteamWorksUserUtils::ProfileColor(UTexture2D* avatar, FLinearColor& outColor)
{
	if (!avatar || !avatar->GetPlatformData() || avatar->GetPlatformData()->Mips.Num() == 0)
		return false;

	FTexture2DMipMap& Mip = avatar->GetPlatformData()->Mips[0];
	const int32 Width = Mip.SizeX;
	const int32 Height = Mip.SizeY;

	FByteBulkData* RawImageData = &Mip.BulkData;
	if (!RawImageData)
		return false;

	FColor* Pixels = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));
	if (!Pixels)
		return false;

	const int32 NumPixels = Width * Height;

	// Histogram for hue (0–360 mapped into bins)
	const int32 HueBins = 36; // 10° per bin
	float Histogram[HueBins] = {0};

	for (int32 i = 0; i < NumPixels; ++i)
	{
		const FColor& C = Pixels[i];
		FLinearColor Linear = C.ReinterpretAsLinear();

		FLinearColor hsv = Linear.LinearRGBToHSV();
		float H = hsv.R;
		float S = hsv.G;
		float V = hsv.B;
		// Ignore very dark or desaturated pixels (noise/background)
		if (V < 0.1f || S < 0.1f)
			continue;

		int32 Bin = FMath::Clamp(int32(H / 360.f * HueBins), 0, HueBins - 1);

		// Weight by saturation * value for stronger colors
		Histogram[Bin] += S * V;
	}

	RawImageData->Unlock();

	// Find dominant hue bin
	int32 BestBin = 0;
	float MaxValue = 0.f;
	for (int32 i = 0; i < HueBins; ++i)
	{
		if (Histogram[i] > MaxValue)
		{
			MaxValue = Histogram[i];
			BestBin = i;
		}
	}

	FLinearColor outClr;
	outClr.R = (BestBin + 0.5f) / HueBins * 360.f;
	outClr.G = 1.f;
	outClr.B = 1.f;
	outClr.A = 1.f;
	outColor = outClr.HSVToLinearRGB();

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