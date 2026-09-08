// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SCharacterProfileWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterProfileWidget)
		: _BackgroundBrush(nullptr)
		, _BoxBackgroundBrush(nullptr)
		, _CharacterPortraitBrush(nullptr)
		, _LoadingVisibility(EVisibility::Visible)
	{}

	SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush)
	SLATE_ARGUMENT(const FSlateBrush*, BoxBackgroundBrush)
	SLATE_ARGUMENT(const FSlateBrush*, CharacterPortraitBrush)
	SLATE_ATTRIBUTE(FText, CharacterName)
	SLATE_ATTRIBUTE(FText, CharacterAge)
	SLATE_ATTRIBUTE(FText, CharacterJob)
	SLATE_ATTRIBUTE(FText, CharacterDescription)
	SLATE_ATTRIBUTE(EVisibility, LoadingVisibility)

SLATE_END_ARGS()

	static TSharedRef<SCharacterProfileWidget> ConstructRandom();
	
void Construct(const FArguments& InArgs);
};