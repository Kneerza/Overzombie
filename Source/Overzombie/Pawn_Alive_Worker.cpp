// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Alive_Worker.h"
#include "OverzombieController.h"
#include "TaskManagerComponent.h"
#include "Duty.h"
#include "Task.h"
#include "AI_Duty.h"
#include "AI_Alive_Worker.h"
#include "Pawn_Zombie.h"
#include "AI_Zombie.h"

void APawn_Alive_Worker::BeginPlay()
{
	Super::BeginPlay();

	// ...

	InitializeAI();
}

void APawn_Alive_Worker::BeginHearing(AActor* ActorInRange)
{
	Super::BeginHearing(ActorInRange);

	auto ZombieInRange = Cast<APawn_Zombie>(ActorInRange);
	if ((ZombieInRange) && (AI_Alive_Worker_Ref)) {
		AI_Alive_Worker_Ref->RunFromZombies();
	}
}


void APawn_Alive_Worker::EndHearing(AActor* ActorOutOfRange)
{
	Super::EndHearing(ActorOutOfRange);

	auto ZombieOutOfRange = Cast<APawn_Zombie>(ActorOutOfRange);

	if ((ZombieOutOfRange) && (AI_Alive_Worker_Ref)) {
		AI_Alive_Worker_Ref->RunFromZombies();
	}
}

void APawn_Alive_Worker::InitializeAI()
{
	if (AI_Possessing_Class) {

		AI_Alive_Worker_Ref = Cast<AAI_Alive_Worker>(GetWorld()->SpawnActor(AI_Possessing_Class));
		AI_Alive_Worker_Ref->RunAwayDistanceModifier = RunAwayDistanceModifier;
		AI_Alive_Worker_Ref->PointOfInterest_MaxDistance = PointOfInterest_MaxDistance;
		AI_Alive_Worker_Ref->PointOfInterest_MinDistance = PointOfInterest_MinDistance;
	}
	else
	{
		return;
	}

	if (AI_Alive_Worker_Ref)
	{
		AI_Alive_Worker_Ref->Possess(this);
		AI_Alive_Worker_Ref->PossessedAliveWorker_Ref = this;
		AI_Alive_Worker_Ref->ActivateAI();

		if (RunningSpeedToOverride != 0)AI_Alive_Worker_Ref->RunningSpeed = RunningSpeedToOverride;
		if (WalkingSpeedToOverride != 0)AI_Alive_Worker_Ref->WalkingSpeed = WalkingSpeedToOverride;
		if (FloatingSpeedToOverride != 0)AI_Alive_Worker_Ref->FloatingSpeed = FloatingSpeedToOverride;

		AI_Alive_Ref = AI_Alive_Worker_Ref;
	}
}

void APawn_Alive_Worker::Kill()
{
	if (IsValid(AI_Alive_Worker_Ref)) {
		auto CurrentDuty = AI_Alive_Worker_Ref->GetCurrentDuty();
		if (IsValid(CurrentDuty)) {
			auto Duty_AI = CurrentDuty->ControllingAI;
			if (IsValid(Duty_AI))		// TODO Move some AI functionality to AI instead of pawn
			{
				///Duty_AI->RemoveEnlistedSoldier(AI_Alive_Worker_Ref);
			}
		}
		AI_Alive_Worker_Ref->TaskManagerComponent->RemoveFromAvailableWorkers(AI_Alive_Worker_Ref);
		OverzombieController_Ref->AllWorkers.Remove(AI_Alive_Worker_Ref);
		if (IsValid(AI_Alive_Worker_Ref))if (IsValid(AI_Alive_Worker_Ref->GetCurrentTask()))
		//	AI_Alive_Soldier_Ref->GetCurrentTask()->DismissSoldier(AI_Alive_Soldier_Ref);
		if (IsValid(AI_Alive_Worker_Ref))AI_Alive_Worker_Ref->ReleaseTask();
		if (IsValid(AI_Alive_Worker_Ref))AI_Alive_Worker_Ref->Destroy();
		UE_LOG(LogTemp, Error, TEXT("ATTN:                   AI IS DESTROYED"))
	}

	Super::Kill();
}
