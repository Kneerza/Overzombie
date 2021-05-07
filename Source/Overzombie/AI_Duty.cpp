// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Duty.h"
#include "Pawn_Alive_Soldier.h"
#include "AI_Alive_Soldier.h"
#include "Engine/TargetPoint.h"
#include "Duty.h"
#include "EngineUtils.h"

AAI_Duty::AAI_Duty()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAI_Duty::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	
}

void AAI_Duty::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Error, TEXT("Current patrolling point: %s"), *CurrentPatrollingPoint->GetName())

	if (isOverwatching)
	{
		if ((GetWorld()->GetTimeSeconds() - StartOverwatchingTime) > OverwatchingTime)
		{
			isOverwatching = false;
			ChangeCurrentPatrollingPoint();
		}
		return;
	}

	if (isWaiting)
	{
		UE_LOG(LogTemp, Error, TEXT("Is waiting"))
		StopMovement();
		//if (EnlistedSoldiers.Num() > 0) {
			if (AvailableSlotsCount < DutySlots.Num()) {
			for (int32 i = 0; i < EnlistedSoldiers.Num(); i++)
			{
				if (!IsValid(EnlistedSoldiers[i]))continue;
				if ((EnlistedSoldiers[i]->CurrentBehaviorMode == EBehaviorMode::Shooting))break;
				if (i == (EnlistedSoldiers.Num() - 1))
				{
					isWaiting = false;
					if(IsValid(CurrentPatrollingPoint))
					MoveToLocation(CurrentPatrollingPoint->GetActorLocation());
				}
			}
		}
		else {
				isWaiting = false;
		}
		return;
	}

		//if (CurrentPatrollingPoint) {
			
		//}
		if (IsValid(PossessedDuty) && IsValid(CurrentPatrollingPoint)) {
			if ((PossessedDuty->GetActorLocation() - CurrentPatrollingPoint->GetActorLocation()).Size() < 250)
			{
				ChangeCurrentPatrollingPoint();
			}

			//	MoveToLocation(CurrentPatrollingPoint->GetActorLocation());
		}
}

void AAI_Duty::EnlistAvailableSoldiers()
{
	if (!hasAvailableSlots)return;
	
	for (TActorIterator<AAI_Alive_Soldier>ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (!ActorItr->GetIsAvailableForDuty())continue;
		if (ActorItr->PossessedAliveSoldier_Ref->isInVehicle)continue;
		if (!ActorItr->GetisOnDuty())
		{
			for (int32 i = 0; i < EnlistedSoldiers.Num(); i++)
			{
			
				if (EnlistedSoldiers[i] != NULL)continue;
					EnlistedSoldiers[i] = *ActorItr;
				
					ActorItr->SetisOnDuty(true);
					ActorItr->SetCurrentDuty(PossessedDuty);

				ActorItr->SetCurrentDutySlot(DutySlots[i]);

				break;
			}

			//AvailableSlotsCount = DutySlots.Num();

			UpdateAvailableSlotsCount();
		}
		
	}
}

void AAI_Duty::EnlistAvailableSoldier(AAI_Alive_Soldier* SoldierToEnlist)
{
	if (!IsValid(SoldierToEnlist))return;
	if (!hasAvailableSlots)return;
	if (SoldierToEnlist->GetisOnDuty())return;
	if (!SoldierToEnlist->GetIsAvailableForDuty())return;
	if (SoldierToEnlist->PossessedAliveSoldier_Ref->isInVehicle)return;

	SoldierToEnlist->SetisOnDuty(true);
	SoldierToEnlist->SetCurrentDuty(PossessedDuty);
	

	for (int32 i = 0; i < EnlistedSoldiers.Num(); i++)
	{
		if (EnlistedSoldiers[i] != NULL)continue;
			EnlistedSoldiers[i] = SoldierToEnlist;
		SoldierToEnlist->SetCurrentDutySlot(DutySlots[i]);
		break;
	}

	UpdateAvailableSlotsCount();
}

void AAI_Duty::UpdateAvailableSlotsCount()
{
	AvailableSlotsCount = DutySlots.Num();

	for (int32 i = 0; i < EnlistedSoldiers.Num(); i++)
	{
		if (IsValid(EnlistedSoldiers[i]))AvailableSlotsCount--;
	}


	if (AvailableSlotsCount <= 0)
	{
		hasAvailableSlots = false;
		return;
	}
	else {
		hasAvailableSlots = true;
	}


	 if (AvailableSlotsCount >= DutySlots.Num())
	{
		if (isWaiting)isWaiting = false;
	}
}

void AAI_Duty::InitializeSlots()
{
	if (!IsValid(PossessedDuty))return;
	DutySlots = PossessedDuty->GetDutySlots();
	AvailableSlotsCount = DutySlots.Num();
	EnlistedSoldiers.Init(NULL, AvailableSlotsCount);
	PatrollingPoints = PossessedDuty->GetPatrollingPoints();
	
	ChangeCurrentPatrollingPoint();

	
}

void AAI_Duty::ChangeCurrentPatrollingPoint()
{
	if (PatrollingPoints.Num() == 0) {
	
			return;
	}
	
	int32 Index = 0;

	if (!IsValid(CurrentPatrollingPoint))
	{
		if (!IsValid(PatrollingPointWhenOverwatchOver))
		{
			CurrentPatrollingPoint = PatrollingPoints[0];
		}
		else {
			CurrentPatrollingPoint = PatrollingPointWhenOverwatchOver;
			MoveToLocation(PatrollingPointWhenOverwatchOver->GetActorLocation());
			PatrollingPointWhenOverwatchOver = NULL;
			UE_LOG(LogTemp, Error, TEXT("Waiting Is OVER"))
			return;
		}
	}
	else if (CurrentPatrollingPoint == PatrollingPoints.Last())
	{
		CurrentPatrollingPoint = PatrollingPoints[0];
	}
	else
	{
		if (PatrollingPoints.Find(CurrentPatrollingPoint, Index)) { CurrentPatrollingPoint = PatrollingPoints[Index + 1]; }
	}
	if (!CurrentPatrollingPoint)
	{
		isOverwatching = true;
		StartOverwatchingTime = GetWorld()->GetTimeSeconds();

		Index += 2;
		if (!PatrollingPoints.IsValidIndex(Index))Index = 0;
		PatrollingPointWhenOverwatchOver = PatrollingPoints[Index];

		return;
	}
	MoveToLocation(CurrentPatrollingPoint->GetActorLocation());
}

// --- Access to variables ---

void AAI_Duty::SetPossessedDuty(ADuty* NewPossessedDuty)
{
	if (!IsValid(NewPossessedDuty))return;
	PossessedDuty = NewPossessedDuty;
	InitializeSlots();
	EnlistAvailableSoldiers();
}

void AAI_Duty::SetHasAvailableSlots(bool NewhasAvailableSlots)
{
	hasAvailableSlots = NewhasAvailableSlots;
	if (NewhasAvailableSlots)EnlistAvailableSoldiers();
}

bool AAI_Duty::GetHasAvailableSlots()
{
	return hasAvailableSlots;
}

void AAI_Duty::RemoveEnlistedSoldier(AAI_Alive_Soldier* SoldierToRemove)
{
	if (EnlistedSoldiers.Contains(SoldierToRemove)) {
		auto SoldierIndex = EnlistedSoldiers.Find(SoldierToRemove);
		EnlistedSoldiers[SoldierIndex] = NULL;
		//SetHasAvailableSlots(true);
		UpdateAvailableSlotsCount();
		EnlistAvailableSoldiers();
		UpdateAvailableSlotsCount();
	}
}

void AAI_Duty::SetOverwatchingTime(float NewOverwatchingTime)
{
	OverwatchingTime = NewOverwatchingTime;
}

int32 AAI_Duty::GetAvailableSlotsCount()
{
	return AvailableSlotsCount;
}
// ------