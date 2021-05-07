// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Zombie.h"
#include "Components/CapsuleComponent.h"
#include "PsychoSphere.h"
#include "Pawn_Zombie.h"
#include "Pawn_Alive_Civilian.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "OverzombieController.h"
#include "Engine/TargetPoint.h"
#include "DrawDebugHelpers.h"
#include "AI_Overzombie_Alive.h"

void AAI_Zombie::BeginPlay()
{
	Super::BeginPlay();

	// ...

	PsychoSphere = OverzombieController_Ref->PsychoSphere;
}

void AAI_Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFalling)
	{
		if (!PossessedZombie_Ref)
		{
			Destroy();
			return;
		}

		if (PossessedZombie_Ref->GetVelocity().Size() < 3) {
			Possess(PossessedZombie_Ref);
			PossessedZombie_Ref->Capsule->SetSimulatePhysics(false);
			PossessedZombie_Ref->Capsule->SetEnableGravity(false);

			PossessedZombie_Ref->SetActorEnableCollision(false);

			Elevation = OverzombieController_Ref->GetClosestFloorElevation(PossessedZombie_Ref->GetActorLocation().Z) + ZombiePivotOffset;

			auto rot = PossessedZombie_Ref->GetActorRotation();
			PossessedZombie_Ref->SetActorRotation(FRotator(0));

			auto NewLoc = PossessedZombie_Ref->GetActorLocation();
			NewLoc.Z = Elevation;
			PossessedZombie_Ref->SetActorLocation(NewLoc);

			SetIsFalling(false);
			isRaising = true;
		}

		return;
	}
	else if (isRaising)
	{
		PossessedZombie_Ref->SetActorEnableCollision(true);

		///PossessedZombie_Ref->SetActorRotation(RotationBeforeFalling);

		TurnToLocation(PossessedZombie_Ref->GetActorLocation() + DirBeforeFalling * 100.f);

		if (GetWorld()->GetTimeSeconds() - StartChargingTime <= CurrentChargingTime)
		{
			UE_LOG(LogTemp, Warning, TEXT("----------------------------------------------- Charging AFTER FALLING"))

			ChangeBehavior(EBehaviorMode::ChargeForward);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("----------------------------------------------- NOT Charging AFTER FALLING"))

			ChangeBehavior(EBehaviorMode::Idle);
		}

		isRaising = false;
		return;
	}
	if (!isUnderControl) {
		if (AllAlives_Hearing.Num() > 0)
		{
			float bufClosestDistance = 10000;

			AliveToConsume = NULL;

			for (int32 i = 0; i < AllAlives_Hearing.Num(); i++)
			{
				if (!IsValid(AllAlives_Hearing[i]))continue;

				float ZombieToAliveDistance = (AllAlives_Hearing[i]->GetActorLocation() - PossessedZombie_Ref->GetActorLocation()).Size();
				if (ZombieToAliveDistance < bufClosestDistance)
				{
					bufClosestDistance = ZombieToAliveDistance;

					AliveToConsume = AllAlives_Hearing[i];
				}
			}
		}

		if (IsValid(AliveToConsume)) {
			auto NewLoc = AliveToConsume->GetActorLocation();
			NewLoc.Z = Elevation;

			MoveToLocation(NewLoc, -1.f, true, false);

			if ((AliveToConsume->GetActorLocation() - PossessedZombie_Ref->GetActorLocation()).Size() <= BitingDistance)
			{

				if (GetWorld()->GetTimeSeconds() - TimeOfLastBite > RechargingTime) {
					if (AliveToConsume) {
						Bite();
					}
				}
			}
			if (CurrentBehaviorMode != EBehaviorMode::Pursuing) {
				ChangeBehavior(EBehaviorMode::Pursuing);
			}
		}
		else if (CurrentBehaviorMode == EBehaviorMode::Pursuing) {
			ChangeBehavior(EBehaviorMode::Idle);
		}
	}

	if (isUnderControl)
	{
		LastPsychoSphereLoc = PsychoSphere->GetActorLocation() + OffsetFromCurrentTarget;
		LastPsychoSphereLoc.Z = Elevation;
		MoveToLocation(LastPsychoSphereLoc, -1.f, true, false);
	}
	else {
		switch (CurrentBehaviorMode)
		{
		case EBehaviorMode::Purpose:

			if ((PossessedZombie_Ref->GetActorLocation() - LastPsychoSphereLoc).Size() <= 200.f)
			{
				ChangeBehavior(EBehaviorMode::Idle);
				FloatInRandomDirection();
			}

			break;

		case EBehaviorMode::ChargeForward:

			if (GetWorld()->GetTimeSeconds() - StartChargingTime >= CurrentChargingTime) ChangeBehavior(EBehaviorMode::Idle);
			else
			{
				PossessedZombie_Ref->AddMovementInput(PossessedZombie_Ref->GetActorForwardVector());
			}

			break;

		default:

			if (((PossessedZombie_Ref->GetActorLocation() - PointOfInterest).Size() < 200.f) || (GetWorld()->GetTimeSeconds() > TimeBeforeSetNewPointOfInterest))
			{
				FloatInRandomDirection();
			}

			break;
		}
	}
}

void AAI_Zombie::ActivateAI()
{
	IsActive = true;

	if (PossessedZombie_Ref)
	{
		Elevation = OverzombieController_Ref->GetClosestFloorElevation(PossessedZombie_Ref->GetActorLocation().Z) + ZombiePivotOffset;

		if(PossessedZombie_Ref->FloatingSpeedToOverride!=0)FloatingSpeed = PossessedZombie_Ref->FloatingSpeedToOverride;
		else FloatingSpeed = 100;
		if (PossessedZombie_Ref->RunningSpeedToOverride != 0)RunningSpeed = PossessedZombie_Ref->RunningSpeedToOverride;
		else RunningSpeed = 800;

		ChangeBehavior(EBehaviorMode::Idle);
	}
	else
	{
		Destroy();
	}

	OverzombieController_Ref->NEW_AllZombies.AddUnique(this);
}

void AAI_Zombie::ChangeBehavior(EBehaviorMode NewBehaviorMode)
{
	CurrentBehaviorMode = NewBehaviorMode;
	switch (CurrentBehaviorMode)
	{
	case EBehaviorMode::Idle:
			
		CurrentBehaviorMode = EBehaviorMode::Idle;

		PossessedZombie_Ref->MovementComponent->MaxSpeed = FloatingSpeed + SpeedBuff;

		FloatInRandomDirection();

			break;

	case EBehaviorMode::Pursuing:

		PossessedZombie_Ref->MovementComponent->MaxSpeed = RunningSpeed + SpeedBuff;

		PursuingDummyOffset = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f).RotateVector(FVector(0.f, FMath::RandRange(0.f, 300.f), 0.f));

			break;

	case EBehaviorMode::Purpose:

		PossessedZombie_Ref->MovementComponent->MaxSpeed = RunningSpeed + SpeedBuff;

		OffsetFromCurrentTarget = FRotator(0, FMath::RandRange(-180.f, 180.f), 0).RotateVector(FVector(FMath::RandRange(4.f, OverzombieController_Ref->G_HuddleOffsetFromPointOfInterest), 0.f, 0.f));
			
		CurrentBehaviorMode = EBehaviorMode::Purpose;

			break;

	case EBehaviorMode::ChargeForward:

		PossessedZombie_Ref->MovementComponent->StopActiveMovement();
		PossessedZombie_Ref->MovementComponent->MaxSpeed = RunningSpeed + SpeedBuff;

		CurrentChargingTime = ChargingTime;

		StartChargingTime = GetWorld()->GetTimeSeconds();

		break;

	default:
			break;
	}
}

void AAI_Zombie::SetPointOfInterest()
{
	bool IsPointOfInterestSet = false;
	int32 Counter = 0;
	auto ActorDir = PossessedZombie_Ref->GetActorForwardVector();

	auto Rand = FMath::RandRange(-180.f, 180.f);
	FRotator Rotator = FRotator(0.f, Rand, 0.f);
		
	auto PointOfInterestDir = Rotator.RotateVector(ActorDir);

	PointOfInterest = PossessedZombie_Ref->GetActorLocation() - (PointOfInterestDir * FMath::RandRange(350.f, 1000.f));
	PointOfInterest.Z = Elevation;
}

void AAI_Zombie::FloatInRandomDirection()
{
	SetPointOfInterest();

	MoveToLocation(PointOfInterest, -1.f, true, false, true);

	TimeBeforeSetNewPointOfInterest = GetWorld()->GetTimeSeconds() + FMath::RandRange(1.f, 4.f);
}

void AAI_Zombie::FloatToLocation(FVector LocationFloatTo, float Speed)
{
	PossessedZombie_Ref->MovementComponent->MaxSpeed = Speed;

	PointOfInterest = LocationFloatTo;
	PointOfInterest.Z = Elevation;

	PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedZombie_Ref->GetActorLocation(), PointOfInterest);

	PlayerRot = FRotator(0.f, PlayerRot.Yaw, 0.f);
	PossessedZombie_Ref->SetActorRotation(PlayerRot);

	MoveToLocation(PointOfInterest, -1.f, true, false, true);

	TimeBeforeSetNewPointOfInterest = GetWorld()->GetTimeSeconds() + 100.f;
}

void AAI_Zombie::ChargeForward()
{

}

void AAI_Zombie::Bite()
{
	if (!IsValid(AliveToConsume))return;

	if (AliveToConsume->RecieveDamage(Damage))
	{
		if (!AliveToConsume->isDying) {
			if (AliveToConsume) AliveToConsume->isDying = true;
			ChangeBehavior(EBehaviorMode::Idle);

			AllAlives_Hearing.Remove(AliveToConsume);
			OverzombieController_Ref->TurnAliveIntoZombie(AliveToConsume);

			if (IsValid(AliveToConsume)) AliveToConsume->Kill();
			AliveToConsume = NULL;
		}
	}

	TimeOfLastBite = GetWorld()->GetTimeSeconds();
}

void AAI_Zombie::ChangeAliveToConsume()
{
	//----- Remove Alives that are too far

	float MIN_Distance = 10000;

	if (AllAlives_Hearing.Num() == 0) { AliveToConsume = nullptr; return; }
	
	for (int32 i = 0; i < AllAlives_Hearing.Num(); i++)
	{
		if (!AllAlives_Hearing[i])continue;
		auto CurrentDistance = (PossessedZombie_Ref->GetActorLocation() - AllAlives_Hearing[i]->GetActorLocation()).Size();
		if ((PossessedZombie_Ref->GetActorLocation() - AllAlives_Hearing[i]->GetActorLocation()).Size() < MIN_Distance)
		{
			AliveToConsume = AllAlives_Hearing[i];
			MIN_Distance = CurrentDistance;
		}
	}
}

//-------------- Same as ChangeAliveToConsume() for now
void AAI_Zombie::GetClosestAlive() 
{
	//----- Remove Alives that are too far
	AliveToConsume = nullptr;
	if (AllAlives_Hearing.Num() != 0)
	{
		for (int32 i = (AllAlives_Hearing.Num()-1); i >= 0; i--)
		{
			if (!AllAlives_Hearing[i]) AllAlives_Hearing.Remove(AllAlives_Hearing[i]);
			if ((PossessedZombie_Ref->GetActorLocation() - AliveToConsume->GetActorLocation()).Size() > NotFollowDistance)AllAlives_Hearing.Remove(AllAlives_Hearing[i]);	
		}
	}

	float MIN_Distance = 10000;

	if (AllAlives_Hearing.Num() != 0)
	{
		for (int32 i = 0; i < AllAlives_Hearing.Num(); i++)
		{
			if (AllAlives_Hearing[i])continue;
			auto CurrentDistance = (PossessedZombie_Ref->GetActorLocation() - AllAlives_Hearing[i]->GetActorLocation()).Size();
			if ((PossessedZombie_Ref->GetActorLocation() - AllAlives_Hearing[i]->GetActorLocation()).Size() < MIN_Distance)
			{
				AliveToConsume = AllAlives_Hearing[i];
				MIN_Distance = CurrentDistance;
			}
		}
	}
}
void AAI_Zombie::ApplySpeedBuff(float SpeedBuffAmount)
{
	SpeedBuff = SpeedBuffAmount;
	PossessedZombie_Ref->MovementComponent->MaxSpeed += SpeedBuffAmount;
}

void AAI_Zombie::RemoveSpeedBuff()
{
	PossessedZombie_Ref->MovementComponent->MaxSpeed -= SpeedBuff;
	SpeedBuff = 0;
}

void AAI_Zombie::SetIsFalling(bool NewIsFalling)
{
	if (isFalling == NewIsFalling)return;

	isFalling = NewIsFalling;

	if (IsValid(PossessedZombie_Ref))
	{
		if (NewIsFalling)
		{
			DirBeforeFalling = PossessedZombie_Ref->GetActorForwardVector();
		}
		
		PossessedZombie_Ref->isFalling = NewIsFalling;
	}
}

bool AAI_Zombie::GetIsFalling()
{
	return isFalling;
}

void AAI_Zombie::TurnToLocation(FVector LocationTurnTo)
{
	FRotator bufPlayerRot;
	bufPlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedZombie_Ref->GetActorLocation(), LocationTurnTo);
	PossessedZombie_Ref->SetActorRotation(FRotator(0.f, bufPlayerRot.Yaw, 0.f));
}