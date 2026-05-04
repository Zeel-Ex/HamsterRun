// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/BaseUtils.h"

#include "IAssetTools.h"
#include "Engine/TextureRenderTarget2D.h"

UTexture2D* UBaseUtils::GetTextureRenderTarget(UTextureRenderTarget2D* RTarget)
{
	if (!RTarget->IsValidLowLevel()) return nullptr;
	
	FString Name;
	FString PackageName;
	IAssetTools::Get().CreateUniqueAssetName(RTarget->GetOutermost()->GetName(), TEXT("_Tex"), PackageName, Name);

	UTexture2D* Texture2D = RTarget->ConstructTexture2D(CreatePackage(*PackageName), Name, RTarget->GetMaskedFlags(), CTF_Default);
	RTarget->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
#if WITH_EDITORONLY_DATA
	Texture2D->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif
	Texture2D->SRGB = 1;
	Texture2D->UpdateResource();

	return Texture2D;
}
