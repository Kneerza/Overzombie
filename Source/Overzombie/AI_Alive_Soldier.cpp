// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Alive_Soldier.h"
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

void AAI_Alive_Soldier::ActivateAI()
{
	
	IsActive = true;

	if (PossessedAliveSoldier_Ref->FloatingSpeedToOverride != 0)FloatingSpeed = PossessedAliveSoldier_Ref->FloatingSpeedToOverride;
	else FloatingSpeed = 450;
	if (PossessedAliveSoldier_Ref->WalkingSpeedToOverride != 0)WalkingSpeed = PossessedAliveSoldier_Ref->WalkingSpeedToOverride;
	else WalkingSpeed = 550;
	if (PossessedAliveSoldier_Ref->RunningSpeedToOverride != 0)RunningSpeed = PossessedAliveSoldier_Ref->RunningSpeedToOverride;
	else RunningSpeed = 1450;

	ChangeBehavior(EBehaviorMode::Idle);
	PossessedAliveSoldier_Ref->MovementComponent->MaxSpeed = WalkingSpeed;
	OverzombieController_Ref->AllSoldiers.AddUnique(this);

	TaskManagerComponent->AddToAvailableSoldiers(this);
	EnlistOnAvailableDuty();	
}

void AAI_Alive_Soldier::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	switch (CurrentBehaviorMode)
	{
	case EBehaviorMode::WorkingOnTask:

		if (!IsValid(CurrentSingleTask)) { ChangeBehavior(EBehaviorMode::Idle); break; }

		if ((GetWorld()->GetTimeSeconds() - LastWorkProgressTime) >= WorkProgressRateSeconds) {

			LastWorkProgressTime = GetWorld()->GetTimeSeconds();
			if (CurrentSingleTask->AddTaskProgress(WorkEfficiency))
			{
				if (IsValid(CurrentSingleTask)) {
					CurrentSingleTask->AssignedSoldier = NULL;
					CurrentSingleTask = NULL;
				}

				ChangeBehavior(EBehaviorMode::Idle);
			}
			
		}

		break;

	case EBehaviorMode::WorkingOnInfiniteTask:

		if (!IsValid(CurrentSingleTask)) { ChangeBehavior(EBehaviorMode::Idle); break; }

		break;

	case EBehaviorMode::GoingToTask:

		TurnToLocation(PossessedAliveSoldier_Ref->GetActorLocation() + PossessedAliveSoldier_Ref->GetVelocity()*1000.f);

		break;

	default:
		break;
	}
	
	if (!IsValid(CurrentShootingTarget) && AllZombies_Hearing.Num() > 0)
	{
		for (int32 i = 0; i < AllZombies_Hearing.Num(); i++)
		{
			///if ((PossessedAliveSoldier_Ref->GetActorLocation() - AllZombies_Hearing[i]->GetActorLocation()).Size() <= ReactionDistance) {
				///TurnToActor(AllZombies_Hearing[i]);
				
			///}
		}
	}
	
	CurrentShootingTarget = GetClosestZombie_Hearing();
	
	
	if (CurrentShootingTarget) {
		StopMovement();
		TurnToActor(CurrentShootingTarget);
		if (CurrentBehaviorMode != EBehaviorMode::Shooting) {
			ChangeBehavior(EBehaviorMode::Shooting);
			
		}
	}
	else if (IsValid(CurrentDuty))
	{
		if (CurrentBehaviorMode != EBehaviorMode::GoingOnDuty)
		{
			if (CurrentBehaviorMode == EBehaviorMode::OnDuty)
			{
				if ((PossessedAliveSoldier_Ref->GetActorLocation() - CurrentDutySlot->GetActorLocation()).Size() <= 300.f)
				{
					
					PossessedAliveSoldier_Ref->MovementComponent->MaxSpeed = CurrentDuty->DutySpeed;// WalkingSpeed;
				}
				else {
					PossessedAliveSoldier_Ref->MovementComponent->MaxSpeed = RunningSpeed;
				}
				MoveToActor(CurrentDutySlot);
			}
			else {
				ChangeBehavior(EBehaviorMode::GoingOnDuty);
			}
		}
		else {
			if (IsValid(PossessedAliveSoldier_Ref) && IsValid(CurrentDutySlot)) {
				if ((PossessedAliveSoldier_Ref->GetActorLocation() - CurrentDutySlot->GetActorLocation()).Size() < 300.f)
				{
						ChangeBehavior(EBehaviorMode::OnDuty);
				}
			}
		}
		
		if(IsValid(CurrentDutySlot))TurnToLocation(CurrentDutySlot->GetActorLocation());
		
	}
	else if (CurrentBehaviorMode == EBehaviorMode::Shooting)
	{
		ChangeBehavior(EBehaviorMode::Idle);

	}

	if (CurrentBehaviorMode == EBehaviorMode::Shooting)
	{
		Shoot();
	}
}

void AAI_Alive_Soldier::ChangeBehavior(EBehaviorMode NewBehaviorMode)
{
	
	if (!PossessedAliveSoldier_Ref) { return; }

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
			TaskManagerComponent->AddToAvailableSoldiers(this);
			TaskManagerComponent->AssignMilitaryTask();
		}

		break;

	case EBehaviorMode::Pursuing:

		break;

	case EBehaviorMode::Purpose:

	case EBehaviorMode::Shooting:

		if (IsValid(CurrentDuty))
		{
			CurrentDuty->ControllingAI->isWaiting = true;
		}

		break;

	case EBehaviorMode::GoingOnDuty:
		if (CurrentDutySlot) {
			PossessedAliveSoldier_Ref->MovementComponent->MaxSpeed = RunningSpeed;
			MoveToActor(CurrentDutySlot);
		}

		break;

	case EBehaviorMode::OnDuty:
		if (CurrentDutySlot) {
			PossessedAliveSoldier_Ref->MovementComponent->MaxSpeed = WalkingSpeed;
			MoveToActor(CurrentDutySlot);
		}

		break;

	case EBehaviorMode::GoingToTask:

		if (!IsValid(CurrentSingleTask)) {ChangeBehavior(EBehaviorMode::Idle); break;}
		if (CurrentSingleTask->IsOverlappingActor(PossessedAliveSoldier_Ref)) { ChangeBehavior(EBehaviorMode::WorkingOnTask); break; }
		MoveToLocation(CurrentSingleTask->GetActorLocation());

		break;

	case EBehaviorMode::WorkingOnTask:

		TurnToLocation(CurrentSingleTask->GetActorLocation() + CurrentSingleTask->GetActorForwardVector()*1000.f);

		break;

	case EBehaviorMode::WorkingOnInfiniteTask:

		TurnToLocation(CurrentSingleTask->GetActorLocation() + CurrentSingleTask->GetActorForwardVector()*1000.f);

		break;

	default:
		break;
	}
	

}

APawn_Zombie* AAI_Alive_Soldier::GetClosestZombie_Hearing()
{
	if (AllZombies_Hearing.Num() == 0) return nullptr;
	
	APawn_Zombie* OUT_ClosestZombie = nullptr;

	float CurrentClosestDistance = 10000;
	FVector SoldierToZombieVec = FVector(0);
	auto PossessedSoldierLoc = PossessedAliveSoldier_Ref->GetActorLocation();

	for (int32 i = 0; i < AllZombies_Hearing.Num(); i++)
	{
		// Check if Zombie is in Hearing Radius
		SoldierToZombieVec = PossessedSoldierLoc - AllZombies_Hearing[i]->GetActorLocation();
		SoldierToZombieVec.Z = 0;

		float SoldierToZombieDistance = SoldierToZombieVec.Size();
				
		if (SoldierToZombieDistance < CurrentClosestDistance)
		{
		OUT_ClosestZombie = AllZombies_Hearing[i];
			CurrentClosestDistance = SoldierToZombieDistance;
		}
	}
	
	return OUT_ClosestZombie;
}

void AAI_Alive_Soldier::Shoot()
{
	if (!CurrentShootingTarget)return;

	auto CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentTime - LastShotTime >= ReloadingSpeed)
	{
		if (StartAimingTimeSeconds == 0.f) {
			StartAimingTimeSeconds = CurrentTime;
			return;
		}
		else if ((CurrentTime - StartAimingTimeSeconds) < AimingTime)
		{
			return;
		}

		TArray<APawn_Zombie*> ZombiesToShoot;
		ZombiesToShoot.AddUnique(CurrentShootingTarget);

		float TargetHP = 0;
		auto bufPossessedAliveSoldierLocation = PossessedAliveSoldier_Ref->GetActorLocation();

		LastShotTime = CurrentTime;
		StartAimingTimeSeconds = 0.f;
		TurnTowardsNoise(CurrentShootingTarget);

		for (int32 i = 0; i < ZombiesToShoot.Num(); i++)
		{
			if (!IsValid(ZombiesToShoot[i]))continue;
			if ((bufPossessedAliveSoldierLocation - ZombiesToShoot[i]->GetActorLocation()).Size() > PossessedAliveSoldier_Ref->HearingRadius)continue;/// bufPossessedAliveSoldierVisionRadius)continue;

			TargetHP = ZombiesToShoot[i]->HP;

			TargetHP = (TargetHP - Damage);
			ZombiesToShoot[i]->HP = TargetHP;

			OverzombieController_Ref->Effect_Shoot(PossessedAliveSoldier_Ref->ShootEffectType,FireSocket->GetComponentTransform());
			OverzombieController_Ref->Effect_Bleed(ZombiesToShoot[i]->GetActorLocation());

			if (ZombiesToShoot[i]->HP <= 0)
			{
				ZombiesToShoot[i]->Kill();
			}
		}
	}
}

void AAI_Alive_Soldier::TurnToActor(AActor* ActorToTurnTo)
{
	auto ActorToTurnToLocation = ActorToTurnTo->GetActorLocation();
	FRotator PlayerRot;
	PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveSoldier_Ref->GetActorLocation(), ActorToTurnToLocation);
	PossessedAliveSoldier_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
}

void AAI_Alive_Soldier::EnlistOnAvailableDuty()
{
	if (!isAvailableForDuty)return;
	if (PossessedAliveSoldier_Ref->isInVehicle)return;
	if (isOnDuty)return;
	for (TActorIterator<AAI_Duty>ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (!ActorItr->GetHasAvailableSlots())continue;
		ActorItr->EnlistAvailableSoldier(this);
		return;
	}
}

void AAI_Alive_Soldier::TurnToLocation(FVector LocationTurnTo)
{
	FRotator PlayerRot;
	PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedAliveSoldier_Ref->GetActorLocation(), LocationTurnTo);
	PossessedAliveSoldier_Ref->SetActorRotation(FRotator(0.f, PlayerRot.Yaw, 0.f));
}


// --- Access to variables ---

bool AAI_Alive_Soldier::GetisOnDuty()
{
	return isOnDuty;
}

void AAI_Alive_Soldier::SetisOnDuty(bool NewisOnDuty)
{
	isOnDuty = NewisOnDuty;
}

void AAI_Alive_Soldier::SetCurrentDuty(ADuty* NewCurrentDuty)
{
	if(IsValid(NewCurrentDuty))CurrentDuty = NewCurrentDuty;
}

ADuty* AAI_Alive_Soldier::GetCurrentDuty()
{
	return CurrentDuty;
}

void AAI_Alive_Soldier::SetCurrentDutySlot(ATargetPoint* NewCurrentDutySlot)
{
	if (NewCurrentDutySlot)CurrentDutySlot = NewCurrentDutySlot;
}

void AAI_Alive_Soldier::SetupWeapon(float NewReloadingSpeed, float NewDamage, bool NewIsSquareDamage, float NewWeaponNoiseRadius, float NewAimingTime)
{
	ReloadingSpeed = NewReloadingSpeed;
	isSquareDamage = NewIsSquareDamage;
	WeaponNoiseRadius = NewWeaponNoiseRadius;
	AimingTime = NewAimingTime;
	Damage = NewDamage;
}

void AAI_Alive_Soldier::SetIsAvailableForDuty(bool NewIsAvailableForDuty)
{
	isAvailableForDuty = NewIsAvailableForDuty;
}

bool AAI_Alive_Soldier::GetIsAvailableForDuty()
{
	return isAvailableForDuty;
}

void AAI_Alive_Soldier::SetReactionDistance(float NewReactionDistance)
{
	ReactionDistance = NewReactionDistance;
}

float AAI_Alive_Soldier::GetReactionDistance()
{
	return ReactionDistance;
}
// ------

void AAI_Alive_Soldier::TurnTowardsNoise(AActor* ActorTurnTo)
{
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(WeaponNoiseRadius);

	FVector bufSourceOfNoiseLocation = PossessedAliveSoldier_Ref->GetActorLocation();

	TArray<FHitResult> OutResults;

	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	// --- Checking StartPoint ---
	GetWorld()->SweepMultiByObjectType(OutResults, bufSourceOfNoiseLocation, bufSourceOfNoiseLocation, FQuat::Identity, Params, CollisionSphere);

	//GetWorld()->SweepMultiByObjectType()

		for (int32 i = 0; i < OutResults.Num(); i++)
		{
			auto AsPawnAliveSoldier = Cast<APawn_Alive_Soldier>(OutResults[i].Actor);
			if (IsValid(AsPawnAliveSoldier))
			{
				if (IsValid(AsPawnAliveSoldier->AI_Alive_Soldier_Ref->CurrentShootingTarget))continue;
				AsPawnAliveSoldier->AI_Alive_Soldier_Ref->TurnToActor(ActorTurnTo);
			}
		}


}

ATask* AAI_Alive_Soldier::GetCurrentTask()
{
	return CurrentTask;
}

void AAI_Alive_Soldier::SetCurrentTask(ATask* NewCurrentTask)
{
	CurrentTask = NewCurrentTask;
}

USlotComponent* AAI_Alive_Soldier::GetCurrentTaskSlot()
{
	return CurrentTaskSlot;
}

void AAI_Alive_Soldier::SetCurrentTaskSlot(USlotComponent* NewCurrentTaskSlot)
{
	CurrentTaskSlot = NewCurrentTaskSlot;
}

void AAI_Alive_Soldier::AssignTask()
{
	TaskManagerComponent->AssignSoldier(this);
}

void AAI_Alive_Soldier::ReleaseTask()
{
	if(IsValid(CurrentSingleTask))CurrentSingleTask->MakeAvailable();
}
