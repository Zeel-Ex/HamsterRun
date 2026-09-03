// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemHelper.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

TMap<AActor*, FTimerHandle> LeafBlowerTimerHandles;
struct FParams
{
	bool ToNormal = false;
};
void UItemHelper::ApplyBlowerForce(const FTransform& BlowerTransform, float MaxRange, float ConeHalfAngleDegrees,
                                   float BaseForceStrength, UObject* WorldContextObject)
{
	FVector Origin = BlowerTransform.GetLocation();
	FVector Forward = BlowerTransform.GetRotation().Vector();
	auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(MaxRange);
	World->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_PhysicsBody, Sphere);

	float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(ConeHalfAngleDegrees));

	bool IsCharacter = false;

	for (const FOverlapResult& Result : Overlaps)
	{
		UPrimitiveComponent* Comp = Result.GetComponent();
		AActor* Actor = Result.GetActor();
		if (Actor)
		{
			IsCharacter = Cast<ACharacter>(Actor) != nullptr;

			if (UFunction* ConvertToDynamic = Actor->FindFunction("I_ConvertToDynamic"))
			{
				struct FParams
				{
					bool ActivateSelfConvert = false;
				};

				FParams Params;
				Actor->ProcessEvent(ConvertToDynamic, &Params);
			}
		}

		if (!IsCharacter)
			if (!Comp || !Comp->IsSimulatingPhysics())
			{
				continue;
			}

		FVector ToTarget = Comp->GetComponentLocation() - Origin;
		float Distance = ToTarget.Size();
		if (Distance < KINDA_SMALL_NUMBER)
		{
			continue;
		}

		FVector Direction = ToTarget / Distance;
		float Dot = FVector::DotProduct(Forward, Direction);

		if (Dot >= CosHalfAngle)
		{
			// Falloff: strongest near the nozzle, fading to zero at MaxRange
			float Falloff = 1.0f - (Distance / MaxRange);
			Falloff = FMath::Clamp(Falloff, 0.0f, 1.0f);

			float AngleFactor = (Dot - CosHalfAngle) / (1.0f - CosHalfAngle);

			FVector Force = Direction * BaseForceStrength * Falloff * AngleFactor;

			if (IsCharacter)
			{
				UCharacterMovementComponent* Movement = Cast<ACharacter>(Actor)->GetCharacterMovement();

				if (UFunction* SetFriction = Actor->FindFunction("SetFriction"))
				{
					FParams Params;
					Actor->ProcessEvent(SetFriction, &Params);

					const auto resetTimer = [=]()
					{
						FParams Params1(true);

						Actor->ProcessEvent(SetFriction, &Params1);
					};

					if (LeafBlowerTimerHandles.Find(Actor)->IsValid())
					{
						World->GetTimerManager().ClearTimer(*LeafBlowerTimerHandles.Find(Actor));
						World->GetTimerManager().SetTimer(*LeafBlowerTimerHandles.Find(Actor), resetTimer, 1, false);
					}
					else
					{
						FTimerHandle TimerHandle;
						World->GetTimerManager().SetTimer(LeafBlowerTimerHandles.Add(Actor, TimerHandle), resetTimer, 1, false);
					}
				}

				Movement->AddForce(Force);
			}
			else
				Comp->AddForce(Force, NAME_None, true);
		}
	}
}
