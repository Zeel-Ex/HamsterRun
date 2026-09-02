#include "SCharacterProfileWidget.h"

#include <unordered_map>

#include "Components/TextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SThrobber.h"

TSharedRef<SCharacterProfileWidget> SCharacterProfileWidget::ConstructRandom()
{
	struct CharacterInfo
	{
		const FSlateBrush* CharacterPortraitBrush;
		FText CharacterName;
		FText CharacterAge;
		FText CharacterJob;
		FText CharacterDescription;
	};
	
	std::unordered_map<int, CharacterInfo> CharacterInfoMap;
	// TODO: relocate the character infos to a more central place 
	CharacterInfo charInfo;

	charInfo.CharacterName = FText::FromString("Leni Streber");
	charInfo.CharacterAge = FText::FromString("30");
	charInfo.CharacterJob = FText::FromString("Moderator");
	charInfo.CharacterDescription = FText::FromString(
		"Leni Streber wollte schon immer ein Gameshow Host sein und ging deshalb auch auf eine Schauspiel-Schule. Um seinen Lebensunterhalt zu verdienen, musste er daraufhin einen Job in einem Einkaufszentrum annehmen.\n\nSeine Leidenschaft lebte er die meiste Zeit in Spiele Abenden mit Freunden aus, die er austrug. An einem diesen Abenden gewann er eine Wette gegen einen seiner Freunde und bekam so endlich einen Job als Host einer Gameshow namens Super Market Mayhem.");
	CharacterInfoMap.emplace(0, charInfo);

	// ConspiracyTheorist = Redneck Joe
	charInfo.CharacterName = FText::FromString("Redneck Joe");
	charInfo.CharacterAge = FText::FromString("53");
	charInfo.CharacterJob = FText::FromString("Bauer");
	charInfo.CharacterDescription = FText::FromString(
		"Joe war ein Farmer und während der Bananenflut wurden all seine Düngemittel und Futter für die Tiere durch Bananen basierte Produkte ausgetauscht. Aus unerfindlichen Gründen funktionierte dies für seinen Hof besser.\n\nDadurch schloss er sich einer Gruppe Verschwörungstheoretiker an, die denken das der Affe eigentlich recht hatte und die Bananenflut wiederkehren soll. Dies will er nun an die Welt heran tragen und macht deshalb bei Super Market Mayhem mit.");
	CharacterInfoMap.emplace(1, charInfo);

	// Grandma = Brigitte Maier (placeholder)
	charInfo.CharacterName = FText::FromString("Brigitte Maier");
	charInfo.CharacterAge = FText::FromString("93 Jahre");
	charInfo.CharacterJob = FText::FromString("Rentnerin");
	charInfo.CharacterDescription = FText::FromString("Nachdem Brigittes Frau gestorben ist zog sie sich sehr zurück und legte sich viele Katzen an. Ihr Alltag wurde immer Monotoner. Dann hat sie in der Zeitung von einer neuen Gameshow gelesen: Super Market Mayhem. Sie meldete sich dort an, um wieder etwas Schwung in ihr Leben zu bekommen.");
	CharacterInfoMap.emplace(2, charInfo);

	// Prepper = Max Clearfield
	charInfo.CharacterName = FText::FromString("Max Clearfield");
	charInfo.CharacterAge = FText::FromString("25");
	charInfo.CharacterJob = FText::FromString("Soldat");
	charInfo.CharacterDescription = FText::FromString(
		"Während der Bananenflut war Maxs Familie einer der wenigen die es nicht für nötig hielten zu hamstern. Seine Mutter wurde daraufhin krank und ihnen fehlten die Medikamente, um sie zu retten.\n\nDanach verschrieb sich Max dem Prepping und begann sich einen Bunker für die nächste Epidemie zu bauen. Als er Erfuhr das es in irgendeinem deutschen Dorf eine Gameshow geben würde in dem man um sonst so viel wie möglich aus einem Laden mitnehmen durfte meldete er sich direkt dafür an.");
	CharacterInfoMap.emplace(3, charInfo);

	// Sleepy = Clara Fischer
	charInfo.CharacterName = FText::FromString("Clara Fischer");
	charInfo.CharacterAge = FText::FromString("21");
	charInfo.CharacterJob = FText::FromString("Studentin");
	charInfo.CharacterDescription = FText::FromString(
		"Clara Fischer ist eine sehr schlaue Studentin, ist jedoch viel zu verschlafen und faul, um groß was zu erreichen. Um sie etwas aufzuwecken und ihr die Konsequenzen ihrer Verschlafenheit zu zeigen, meldeten ihre Freundinnen sie bei einer zufälligen Gameshow im nächsten Ort an: Super Market Mayhem.\n\nSie verschlief nicht nur die Frist, um sich selbst abzumelden sondern auch fast den Start der Dreharbeiten.");
	CharacterInfoMap.emplace(4, charInfo);
			
	CharacterInfo randomChar = CharacterInfoMap.at(FMath::RandRange(0, 4));
	
	return SNew(SCharacterProfileWidget)
		//Character
		.CharacterAge(randomChar.CharacterAge)
		.CharacterDescription(randomChar.CharacterDescription)
		.CharacterJob(randomChar.CharacterJob)
		.CharacterName(randomChar.CharacterName)
		// General Widget
		.BackgroundBrush(new FSlateImageBrush())
}

void SCharacterProfileWidget::Construct(const FArguments& InArgs)
{
	// Helper lambda to construct bright text boxes (Background PNG + Text)
	auto CreateBrightTextBox = [](const FSlateBrush* BackgroundBrush, TAttribute<FText> TextAttr, FVector2D MinimumSize)
	{
		return SNew(SBox)
			.MinDesiredWidth(MinimumSize.X)
			.MinDesiredHeight(MinimumSize.Y)
			[
				SNew(SOverlay)
				// Background Image PNG
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(BackgroundBrush)
				]
				// Foreground Text
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(FMargin(12.0f, 6.0f))
				[
					SNew(STextBlock)
					.Text(TextAttr)
					.Justification(ETextJustify::Center)
				]
			];
	};

	ChildSlot
	[
		SNew(SOverlay)

		// 1. Fullscreen Outer Background
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Image(InArgs._BackgroundBrush)
		]

		// 2. Centered Content with Aspect Ratio Handling
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SScaleBox)
			.Stretch(EStretch::ScaleToFit)
			.StretchDirection(EStretchDirection::DownOnly)
			[
				SNew(SHorizontalBox)

				// LEFT COLUMN: Name Box & Character Image Display
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(20.0f)
				[
					SNew(SVerticalBox)

					// Character Name Box
					+ SVerticalBox::Slot()
					.AutoHeight()
					.HAlign(HAlign_Center)
					.Padding(0.0f, 0.0f, 0.0f, 15.0f)
					[
						CreateBrightTextBox(InArgs._BoxBackgroundBrush, InArgs._CharacterName, FVector2D(180.0f, 40.0f))
					]

					// Character Display Image
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.WidthOverride(260.0f)
						.HeightOverride(400.0f)
						[
							SNew(SImage)
							.Image(InArgs._CharacterPortraitBrush)
						]
					]
				]

				// RIGHT COLUMN: Metadata (Age, Job) & Description
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(20.0f)
				[
					SNew(SVerticalBox)

					// Age & Job Row
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 55.0f, 0.0f, 25.0f)
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(0.0f, 0.0f, 20.0f, 0.0f)
						[
							CreateBrightTextBox(InArgs._BoxBackgroundBrush, InArgs._CharacterAge, FVector2D(160.0f, 40.0f))
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							CreateBrightTextBox(InArgs._BoxBackgroundBrush, InArgs._CharacterJob, FVector2D(160.0f, 40.0f))
						]
					]

					// Description Box
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.WidthOverride(420.0f)
						.HeightOverride(320.0f)
						[
							SNew(SOverlay)
							+ SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							[
								SNew(SImage)
								.Image(InArgs._BoxBackgroundBrush)
							]
							+ SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(20.0f)
							[
								SNew(STextBlock)
								.Text(InArgs._CharacterDescription)
								.AutoWrapText(true)
							]
						]
					]
				]
			]
		]

		// 3. Bottom-Right Loading Indicator
		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(30.0f)
		[
			SNew(SCircularThrobber)
			.Visibility(InArgs._LoadingVisibility)
		]
	];
}
