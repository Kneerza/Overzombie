// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Alive_Civilian.h"
#include "OverzombieController.h"
#include "DistructibleStructure.h"
#include "Pawn_Alive_Civilian.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Pawn_Zombie.h"


void AAI_Alive_Civilian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentBehaviorMode == EBehaviorMode::Purpose)
	{
		if ((PossessedAliveCivilian_Ref->GetActorLocation() - PointOfInterest).Size() < AttnRadius) {
			if (AllZombies_Hearing.Num() != 0) {
				RunFromZombies(); 
			}else { ChangeBehavior(EBehaviorMode::Idle); }
		}
	}
	
	else if (CurrentBehaviorMode == EBehaviorMode::Idle)
	{
		if ((GetWorld()->GetTimeSeconds() > TimeBeforeSetNewPointOfInterest))
		{
				RoamAround();
		}
	}
}

void AAI_Alive_Civilian::ActivateAI()
{
	IsActive = true;

	FloatingSpeed = 350;
	WalkingSpeed = 500;
	RunningSpeed = 900;

	if (PossessedAliveCivilian_Ref)
	{
		ChangeBehavior(EBehaviorMode::Idle);
		Elevation = OverzombieController_Ref->GetClosestFloorElevation(PossessedAliveCivilian_Ref->GetActorLocation().Z);// +MeshPivotOffset;
	}
	else
	{
		Destroy();
	}

	OverzombieController_Ref->AllCivilians.AddUnique(this);
}

void AAI_Alive_Civilian::RoamAround()
{
	if (!PossessedAliveCivilian_Ref) { return; }
	SetPointOfInterest();

	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveCivilian_Ref->GetActorLocation(), PointOfInterest);

	PlayerRot = FRotator(0.f, PlayerRot.Yaw, 0.f);

	PossessedAliveCivilian_Ref->SetActorRotation(PlayerRot);

	PossessedAliveCivilian_Ref->MovementComponent->MaxSpeed = FloatingSpeed;

	MoveToLocation(PointOfInterest);

	TimeBeforeSetNewPointOfInterest = GetWorld()->GetTimeSeconds() + FMath::RandRange(RoamAround_MinWaitingTimeBeforeChangeingDirection, RoamAround_MaxWaitingTimeBeforeChangeingDirection);
}

void AAI_Alive_Civilian::SetPointOfInterest()
{
	bool IsPointOfInterestSet = false;
	int32 Counter = 0;
	auto ActorDir = PossessedAliveCivilian_Ref->GetActorForwardVector();
	ActorDir.Z = 0;

	auto Rand = FMath::RandRange(-180.f, 180.f);
	FRotator Rotator = FRotator(0.f, Rand, 0.f);

	auto PointOfInterestDir = Rotator.RotateVector(ActorDir);
	PointOfInterest = PossessedAliveCivilian_Ref->GetActorLocation() - (PointOfInterestDir * FMath::RandRange(PointOfInterest_MinDistance, PointOfInterest_MaxDistance));
	PointOfInterest.Z = PossessedAliveCivilian_Ref->GetActorLocation().Z;
}

FVector AAI_Alive_Civilian::GetRunFromZombiesDirection()
{
	if (!PossessedAliveCivilian_Ref) { return FVector(0); }
	if (AllZombies_Hearing.Num() == 0) { return FVector(0); }

	FVector ResultDirection = FVector(0);

	for (int32 i = 0; i < AllZombies_Hearing.Num(); i++)
	{
		if (!AllZombies_Hearing[i]) { continue; }

		ResultDirection = ResultDirection + ((PossessedAliveCivilian_Ref->GetActorLocation() - AllZombies_Hearing[i]->GetActorLocation())*(-1));
	}

	ResultDirection = (ResultDirection / AllZombies_Hearing.Num())*RunAwayDistanceModifier;

	return ResultDirection;
}

void AAI_Alive_Civilian::ChangeBehavior(EBehaviorMode NewBehaviorMode)
{
	if (!PossessedAliveCivilian_Ref) { return; }

	CurrentBehaviorMode = NewBehaviorMode;
	switch (CurrentBehaviorMode)
	{
	case EBehaviorMode::Idle:
		PossessedAliveCivilian_Ref->MovementComponent->MaxSpeed = FloatingSpeed;

		break;

	case EBehaviorMode::Pursuing:
		/*
		PossessedAliveCivilian_Ref->MovementComponent->MaxSpeed = RunningSpeed;
		//MoveToActor(Cast<AActor>(AliveToConsume), 1.f);

		*/
			break;

	case EBehaviorMode::Purpose:
		
		PossessedAliveCivilian_Ref->MovementComponent->MaxSpeed = RunningSpeed;

		MoveToLocation(PointOfInterest);
		//PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveCivilian_Ref->GetActorLocation(), GetRunFromZombiesDirection());

		//PossessedAliveCivilian_Ref->SetActorRotation(PlayerRot);

		//OffsetFromCurrentTarget = FRotator(0, FMath::RandRange(-180.f, 180.f), 0).RotateVector(FVector(FMath::RandRange(4.f, OverzombieController_Ref->G_HuddleOffsetFromPointOfInterest), 0.f, 0.f));
		//MoveToLocation(OverzombieController_Ref->CurrentTarget->GetActorLocation() + OffsetFromCurrentTarget);

		CurrentBehaviorMode = EBehaviorMode::Purpose;
		
		break;
		
	default:
		break;
	}
}

void AAI_Alive_Civilian::RunFromZombies()
{
	if (AllZombies_Hearing.Num() == 0) return;
	if (!PossessedAliveCivilian_Ref) return;

	//----- LineTracingSetup-----

	FHitResult HitResult;
	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Pawn;

	//---------------------------

	ChangeBehavior(EBehaviorMode::Purpose);


	//auto Loc = PossessedAliveCivilian_Ref->GetActorLocation() + (PossessedAliveCivilian_Ref->GetActorLocation() - AllZombies_Hearing[0]->GetActorLocation());

	auto RunFromZombiesDir = GetRunFromZombiesDirection();
	auto PawnLoc = PossessedAliveCivilian_Ref->GetActorLocation();

	PointOfInterest = PawnLoc - RunFromZombiesDir;
	PointOfInterest.Z = PossessedAliveCivilian_Ref->GetActorLocation().Z;

	bool isShorten = false;
	
	float GradIncrement = 40;

	for (int32 i = 0; i < 10/*(360 / GradIncrement)*/; i++)
	{
		//GetWorld()->LineTraceMultiByChannel()
		if (GetWorld()->LineTraceSingleByChannel
		(
			HitResult,
			(PossessedAliveCivilian_Ref->GetActorLocation()),//+FVector(0.f,0.f,-10.f)),
			(PointOfInterest),//+FVector(0.f, 0.f, 10.f)),
			CollisionChannel
		))
		{
	//UE_LOG(LogTemp, Error, TEXT("Hit: %s"),*HitResult.Actor->GetName())
			HitResult.Actor->GetName();	
			auto ReflectedDir = FMath::GetReflectionVector(RunFromZombiesDir.GetSafeNormal(), HitResult.ImpactNormal);
			auto Dot = FVector::DotProduct(HitResult.ImpactNormal, RunFromZombiesDir.GetSafeNormal());
			UE_LOG(LogTemp, Error, TEXT("000000000000000000000000000000000 DOT: %f"), Dot)
			if (FMath::Abs(Dot)>= 0.8f)
			{
				auto bufMod = FMath::RandRange(-1, 1);

				RunFromZombiesDir = FRotator(0.f, 40* bufMod, 0.f).RotateVector(RunFromZombiesDir);

				PointOfInterest = PossessedAliveCivilian_Ref->GetActorLocation() - RunFromZombiesDir;
				PointOfInterest.Z = PossessedAliveCivilian_Ref->GetActorLocation().Z;
			}
			else
			{

				PointOfInterest = HitResult.ImpactPoint - ReflectedDir * ((PawnLoc - PointOfInterest).Size() - (PawnLoc - HitResult.ImpactPoint).Size());
				PointOfInterest.Z = PossessedAliveCivilian_Ref->GetActorLocation().Z;

				PawnLoc = HitResult.ImpactPoint;
			}
		}
		else { 
			MoveToLocation(PointOfInterest);
			FRotator PlayerRot;
			PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveCivilian_Ref->GetActorLocation(), PointOfInterest);
			PossessedAliveCivilian_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
			return;
		}
	}
	MoveToLocation(PointOfInterest);
	
	FRotator PlayerRot;
	PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveCivilian_Ref->GetActorLocation(), PointOfInterest);
	PossessedAliveCivilian_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
}