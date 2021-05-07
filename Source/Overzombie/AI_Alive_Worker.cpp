// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Alive_Worker.h"
#include "Pawn_Alive_Worker.h"
#include "DrawDebugHelpers.h"
#include "TaskManagerComponent.h"
#include "Task.h"
#include "CollisionShape.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "AI_Duty.h"
#include "Duty.h"
#include "Pawn_Zombie.h"
#include "Math/Vector.h"
#include "Pawn_Alive_Soldier.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Math/UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"
#include "OverzombieController.h"
#include "SlotComponent.h"
#include "SingleTaskSlot.h"
#include "DistructibleStructure.h"

void AAI_Alive_Worker::ActivateAI()
{
	IsActive = true;

	if (PossessedAliveWorker_Ref->FloatingSpeedToOverride != 0)FloatingSpeed = PossessedAliveWorker_Ref->FloatingSpeedToOverride;
	else FloatingSpeed = 450;
	if (PossessedAliveWorker_Ref->WalkingSpeedToOverride != 0)WalkingSpeed = PossessedAliveWorker_Ref->WalkingSpeedToOverride;
	else WalkingSpeed = 550;
	if (PossessedAliveWorker_Ref->RunningSpeedToOverride != 0)RunningSpeed = PossessedAliveWorker_Ref->RunningSpeedToOverride;
	else RunningSpeed = 1450;

	ChangeBehavior(EBehaviorMode::Idle);
	PossessedAliveWorker_Ref->MovementComponent->MaxSpeed = WalkingSpeed;
	OverzombieController_Ref->AllWorkers.AddUnique(this);
	//OverzombieController_Ref->AllWos.AddUnique(this);
	TaskManagerComponent->AddToAvailableWorkers(this);
}

void AAI_Alive_Worker::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (CurrentBehaviorMode == EBehaviorMode::Purpose)
	{
		if ((PossessedAliveWorker_Ref->GetActorLocation() - PointOfInterest).Size() < 300.f) {
			if (AllZombies_Hearing.Num() != 0) {
				RunFromZombies();
				return;
			}
			else
			{
				ChangeBehavior(EBehaviorMode::Idle);
			}
		}
	}

	switch (CurrentBehaviorMode)
	{
	case EBehaviorMode::WorkingOnTask:

		if (!IsValid(CurrentSingleTask)) { ChangeBehavior(EBehaviorMode::Idle); break; }

		if ((GetWorld()->GetTimeSeconds() - LastWorkProgressTime) >= WorkProgressRateSeconds) {

			LastWorkProgressTime = GetWorld()->GetTimeSeconds();
			if (CurrentSingleTask->AddTaskProgress(WorkEfficiency))
			{
				if (IsValid(CurrentSingleTask)) {
					CurrentSingleTask->AssignedWorker = NULL;
					CurrentSingleTask->MakeAvailable();
					CurrentSingleTask = NULL;
				}

				ChangeBehavior(EBehaviorMode::Idle);
			}

		}

		break;

	case EBehaviorMode::WorkingOnInfiniteTask:

		break;

	case EBehaviorMode::Waiting:

		if (GetWorld()->GetTimeSeconds() >= EndWaitingTime)
		{
			ChangeBehavior(EBehaviorMode::Idle);
		}

	default:
		break;
	}
}

void AAI_Alive_Worker::ChangeBehavior(EBehaviorMode NewBehaviorMode)
{

	if (!PossessedAliveWorker_Ref) { return; }

	CurrentBehaviorMode = NewBehaviorMode;
	switch (CurrentBehaviorMode)
	{
	case EBehaviorMode::Idle:

		if (IsValid(CurrentSingleTask))
		{
			ChangeBehavior(EBehaviorMode::GoingToTask);
		}
		else
		{
			TaskManagerComponent->AddToAvailableWorkers(this);
			TaskManagerComponent->AssignWorkerTask();
		}

		break;

	case EBehaviorMode::Pursuing:

		break;

	case EBehaviorMode::Purpose:

		PossessedAliveWorker_Ref->MovementComponent->MaxSpeed = RunningSpeed;

		break;

	case EBehaviorMode::Shooting:

		if (IsValid(CurrentDuty))
		{
			CurrentDuty->ControllingAI->isWaiting = true;
		}

		break;

	case EBehaviorMode::GoingOnDuty:
		if (CurrentDutySlot) {
			PossessedAliveWorker_Ref->MovementComponent->MaxSpeed = WalkingSpeed;
			MoveToActor(CurrentDutySlot);
		}

		break;

	case EBehaviorMode::OnDuty:
		if (CurrentDutySlot) {


			PossessedAliveWorker_Ref->MovementComponent->MaxSpeed = WalkingSpeed;
			MoveToActor(CurrentDutySlot);
		}

		break;

	case EBehaviorMode::GoingToTask:

		if (!IsValid(CurrentSingleTask)) { ChangeBehavior(EBehaviorMode::Idle); break; }
		if (CurrentSingleTask->IsOverlappingActor(PossessedAliveWorker_Ref)) { ChangeBehavior(EBehaviorMode::WorkingOnTask); break; }
		MoveToLocation(CurrentSingleTask->GetActorLocation());

		break;

	case EBehaviorMode::WorkingOnTask:

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TEXT("Woker is working on task"));

		TurnToLocation(CurrentSingleTask->GetActorLocation() + CurrentSingleTask->GetActorForwardVector()*1000.f);

	case EBehaviorMode::Waiting:

		EndWaitingTime = GetWorld()->GetTimeSeconds() + WaitingPeriodSeconds;

	default:
		break;
	}


}

APawn_Zombie* AAI_Alive_Worker::GetClosestZombie_Vision()
{


	if (AllZombies_Vision.Num() == 0) return nullptr;

	APawn_Zombie* OUT_ClosestZombie = nullptr;

	float CurrentClosestDistance = 10000;
	FVector SoldierToZombieVec = FVector(0);
	auto PossessedSoldierLoc = PossessedAliveWorker_Ref->GetActorLocation();
	auto PossessedSoldierFrwdVec = PossessedAliveWorker_Ref->GetActorForwardVector()*(-1);


	for (int32 i = 0; i < AllZombies_Vision.Num(); i++)
	{
		// Check if Zombie is in Vision
		SoldierToZombieVec = PossessedSoldierLoc - AllZombies_Vision[i]->GetActorLocation();
		SoldierToZombieVec.Z = 0;
		auto DotProd = FVector::DotProduct(PossessedSoldierFrwdVec, SoldierToZombieVec / SoldierToZombieVec.Size());   //.GetSafeNormal(0.01));
		auto AngleBtwZombieAndSoldier = FMath::RadiansToDegrees(FGenericPlatformMath::Acos(DotProd));

		if (AngleBtwZombieAndSoldier < (VisionAngle / 2))
		{
			// Check if closest

			float SoldierToZombieDistance = SoldierToZombieVec.Size();

			if (SoldierToZombieDistance < CurrentClosestDistance)
			{
				OUT_ClosestZombie = AllZombies_Vision[i];
				CurrentClosestDistance = SoldierToZombieDistance;
			}

		}
		else { continue; }
	}

	return OUT_ClosestZombie;
}

TArray<APawn_Zombie*> AAI_Alive_Worker::GetAllZombies_VisionCone()
{
	TArray<APawn_Zombie*> OUT_Zombies;

	if (AllZombies_Vision.Num() == 0) return OUT_Zombies;

	//APawn_Zombie* OUT_ClosestZombie = nullptr;

	//float CurrentClosestDistance = 10000;
	FVector SoldierToZombieVec = FVector(0);
	auto PossessedSoldierLoc = PossessedAliveWorker_Ref->GetActorLocation();
	auto PossessedSoldierFrwdVec = PossessedAliveWorker_Ref->GetActorForwardVector()*(-1);


	for (int32 i = 0; i < AllZombies_Vision.Num(); i++)
	{
		// Check if Zombie is in Vision
		SoldierToZombieVec = PossessedSoldierLoc - AllZombies_Vision[i]->GetActorLocation();
		SoldierToZombieVec.Z = 0;
		auto DotProd = FVector::DotProduct(PossessedSoldierFrwdVec, SoldierToZombieVec / SoldierToZombieVec.Size());   //.GetSafeNormal(0.01));
		auto AngleBtwZombieAndSoldier = FMath::RadiansToDegrees(FGenericPlatformMath::Acos(DotProd));

		if (AngleBtwZombieAndSoldier < (VisionAngle / 2))
		{
			OUT_Zombies.AddUnique(AllZombies_Vision[i]);
		}
	}

	return OUT_Zombies;
}

void AAI_Alive_Worker::TurnToActor(AActor* ActorToTurnTo)
{
	auto ActorToTurnToLocation = ActorToTurnTo->GetActorLocation();
	FRotator PlayerRot;
	PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveWorker_Ref->GetActorLocation(), ActorToTurnToLocation);
	PossessedAliveWorker_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
}

void AAI_Alive_Worker::EnlistOnAvailableDuty()
{
	if (!isAvailableForDuty)return;
	//if (PossessedAliveWorker_Ref->isInVehicle)return;
	if (isOnDuty)return;
	for (TActorIterator<AAI_Duty>ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (!ActorItr->GetHasAvailableSlots())continue;
	///	ActorItr->EnlistAvailableSoldier(this);
		return;
	}
}

void AAI_Alive_Worker::TurnToLocation(FVector LocationTurnTo)
{
	FRotator PlayerRot;
	PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveWorker_Ref->GetActorLocation(), LocationTurnTo);
	PossessedAliveWorker_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
}


// --- Access to variables ---

bool AAI_Alive_Worker::GetisOnDuty()
{
	return isOnDuty;
}

void AAI_Alive_Worker::SetisOnDuty(bool NewisOnDuty)
{
	isOnDuty = NewisOnDuty;
}

void AAI_Alive_Worker::SetCurrentDuty(ADuty* NewCurrentDuty)
{
	if (IsValid(NewCurrentDuty))CurrentDuty = NewCurrentDuty;
}

ADuty* AAI_Alive_Worker::GetCurrentDuty()
{
	return CurrentDuty;
}

void AAI_Alive_Worker::SetCurrentDutySlot(ATargetPoint* NewCurrentDutySlot)
{
	if (NewCurrentDutySlot)CurrentDutySlot = NewCurrentDutySlot;
}

void AAI_Alive_Worker::SetupWeapon(float NewReloadingSpeed, float NewDamage, bool NewIsSquareDamage, float NewWeaponNoiseRadius, float NewAimingTime)
{
	ReloadingSpeed = NewReloadingSpeed;
	isSquareDamage = NewIsSquareDamage;
	WeaponNoiseRadius = NewWeaponNoiseRadius;
	AimingTime = NewAimingTime;
	Damage = NewDamage;
}

void AAI_Alive_Worker::SetIsAvailableForDuty(bool NewIsAvailableForDuty)
{
	isAvailableForDuty = NewIsAvailableForDuty;
}

bool AAI_Alive_Worker::GetIsAvailableForDuty()
{
	return isAvailableForDuty;
}

void AAI_Alive_Worker::SetReactionDistance(float NewReactionDistance)
{
	ReactionDistance = NewReactionDistance;
}

float AAI_Alive_Worker::GetReactionDistance()
{
	return ReactionDistance;
}
// ------

void AAI_Alive_Worker::TurnTowardsNoise(AActor* ActorTurnTo)
{
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(WeaponNoiseRadius);

	FVector bufSourceOfNoiseLocation = PossessedAliveWorker_Ref->GetActorLocation();

	TArray<FHitResult> OutResults;

	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	// --- Checking StartPoint ---
	GetWorld()->SweepMultiByObjectType(OutResults, bufSourceOfNoiseLocation, bufSourceOfNoiseLocation, FQuat::Identity, Params, CollisionSphere);

	for (int32 i = 0; i < OutResults.Num(); i++)
	{
		auto AsPawnAliveSoldier = Cast<APawn_Alive_Soldier>(OutResults[i].Actor);
		if (IsValid(AsPawnAliveSoldier))
		{
			///if (IsValid(AsPawnAliveSoldier->AI_Alive_Soldier_Ref->CurrentShootingTarget))continue;
			///AsPawnAliveSoldier->AI_Alive_Soldier_Ref->TurnToActor(ActorTurnTo);
		}
	}


}

ATask* AAI_Alive_Worker::GetCurrentTask()
{
	return CurrentTask;
}

void AAI_Alive_Worker::SetCurrentTask(ATask* NewCurrentTask)
{
	CurrentTask = NewCurrentTask;
}

USlotComponent* AAI_Alive_Worker::GetCurrentTaskSlot()
{
	return CurrentTaskSlot;
}

void AAI_Alive_Worker::SetCurrentTaskSlot(USlotComponent* NewCurrentTaskSlot)
{
	CurrentTaskSlot = NewCurrentTaskSlot;
}

void AAI_Alive_Worker::AssignTask()
{
	///TaskManagerComponent->AssignSoldier(this);
}

void AAI_Alive_Worker::ReleaseTask()
{
	if (IsValid(CurrentSingleTask))CurrentSingleTask->MakeAvailable();
	CurrentSingleTask = NULL;
}

FVector AAI_Alive_Worker::GetRunFromZombiesDirection()
{
	if (!PossessedAliveWorker_Ref) { return FVector(0); }
	if (AllZombies_Hearing.Num() == 0) { return FVector(0); }

	FVector ResultDirection = FVector(0);

	for (int32 i = AllZombies_Hearing.Num()-1; i >= 0; i--)
	{
		if (!AllZombies_Hearing[i]) { 
			AllZombies_Hearing.RemoveAt(i);
			continue; 
		}

		ResultDirection = ResultDirection + ((PossessedAliveWorker_Ref->GetActorLocation() - AllZombies_Hearing[i]->GetActorLocation())*(-1));
	}

	ResultDirection = (ResultDirection / AllZombies_Hearing.Num())*RunAwayDistanceModifier;

	return ResultDirection;
}

void AAI_Alive_Worker::RunFromZombies()
{
	if (AllZombies_Hearing.Num() == 0) return;
	if (!PossessedAliveWorker_Ref) return;

	//----- LineTracingSetup-----

	FHitResult HitResult;
	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Pawn;

	//---------------------------

	ChangeBehavior(EBehaviorMode::Purpose);

	auto RunFromZombiesDir = GetRunFromZombiesDirection();
	auto PawnLoc = PossessedAliveWorker_Ref->GetActorLocation();

	PointOfInterest = PawnLoc - RunFromZombiesDir;
	PointOfInterest.Z = PossessedAliveWorker_Ref->GetActorLocation().Z;

	bool isShorten = false;

	float GradIncrement = 40;

	for (int32 i = 0; i < 10; i++)
	{
		if (GetWorld()->LineTraceSingleByChannel
		(
			HitResult,
			(PossessedAliveWorker_Ref->GetActorLocation()),
			(PointOfInterest),
			CollisionChannel
		))
		{
			HitResult.Actor->GetName();
			auto ReflectedDir = FMath::GetReflectionVector(RunFromZombiesDir.GetSafeNormal(), HitResult.ImpactNormal);
			auto Dot = FVector::DotProduct(HitResult.ImpactNormal, RunFromZombiesDir.GetSafeNormal());
			UE_LOG(LogTemp, Error, TEXT("000000000000000000000000000000000 DOT: %f"), Dot)
				if (FMath::Abs(Dot) >= 0.8f)
				{
					auto bufMod = FMath::RandRange(-1, 1);

					RunFromZombiesDir = FRotator(0.f, 40 * bufMod, 0.f).RotateVector(RunFromZombiesDir);

					PointOfInterest = PossessedAliveWorker_Ref->GetActorLocation() - RunFromZombiesDir;
					PointOfInterest.Z = PossessedAliveWorker_Ref->GetActorLocation().Z;
				}
				else
				{

					PointOfInterest = HitResult.ImpactPoint - ReflectedDir * ((PawnLoc - PointOfInterest).Size() - (PawnLoc - HitResult.ImpactPoint).Size());
					PointOfInterest.Z = PossessedAliveWorker_Ref->GetActorLocation().Z;

					PawnLoc = HitResult.ImpactPoint;
				}
		}
		else {
			MoveToLocation(PointOfInterest);
			FRotator PlayerRot;
			PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveWorker_Ref->GetActorLocation(), PointOfInterest);
			PossessedAliveWorker_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
			return;
		}
	}
	MoveToLocation(PointOfInterest);

	FRotator PlayerRot;
	PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveWorker_Ref->GetActorLocation(), PointOfInterest);
	PossessedAliveWorker_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
}