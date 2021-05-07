// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Alive_Civilian.h"
#include "OverzombieController.h"
#include "AI_Alive_Civilian_Dummy.h"
#include "AI_Alive_Civilian.h"
#include "Pawn_Zombie.h"
#include "AI_Zombie.h"

void APawn_Alive_Civilian::BeginPlay()
{
	Super::BeginPlay();

	// ...

			InitializeAI();
}

void APawn_Alive_Civilian::BeginHearing(AActor* ActorInRange)
{
	Super::BeginHearing(ActorInRange);

	auto ZombieInRange = Cast<APawn_Zombie>(ActorInRange);
	if ((ZombieInRange) && (AI_Alive_Civilian_Ref)) {
		AI_Alive_Civilian_Ref->RunFromZombies();
	}
}


void APawn_Alive_Civilian::EndHearing(AActor* ActorOutOfRange)
{
	Super::EndHearing(ActorOutOfRange);

	auto ZombieOutOfRange = Cast<APawn_Zombie>(ActorOutOfRange);

	if ((ZombieOutOfRange) && (AI_Alive_Civilian_Ref)) {
		AI_Alive_Civilian_Ref->RunFromZombies();
	}
}

void APawn_Alive_Civilian::InitializeAI()
{

	if (AI_Possessing_Class) {
		AI_Alive_Civilian_Ref = Cast<AAI_Alive_Civilian>(GetWorld()->SpawnActor(AI_Possessing_Class));
		AI_Alive_Civilian_Ref->RunAwayDistanceModifier = RunAwayDistanceModifier;
		AI_Alive_Civilian_Ref->RoamAround_MaxWaitingTimeBeforeChangeingDirection = RoamAround_MaxWaitingTimeBeforeChangeingDirection;
		AI_Alive_Civilian_Ref->RoamAround_MinWaitingTimeBeforeChangeingDirection = RoamAround_MinWaitingTimeBeforeChangeingDirection;
		AI_Alive_Civilian_Ref->PointOfInterest_MaxDistance = PointOfInterest_MaxDistance;
		AI_Alive_Civilian_Ref->PointOfInterest_MinDistance = PointOfInterest_MinDistance;
	}
	else
	{
		return;
	}

	if (AI_Alive_Civilian_Ref)
	{
		AIasDummy = Cast<AAI_Alive_Civilian_Dummy>(AI_Alive_Civilian_Ref);

		AI_Alive_Civilian_Ref->Possess(this);
		AI_Alive_Civilian_Ref->PossessedAliveCivilian_Ref = this;
		AI_Alive_Civilian_Ref->ActivateAI();

		if (RunningSpeedToOverride!=0)AI_Alive_Civilian_Ref->RunningSpeed = RunningSpeedToOverride;
		if (WalkingSpeedToOverride != 0)AI_Alive_Civilian_Ref->WalkingSpeed = WalkingSpeedToOverride;
		if (FloatingSpeedToOverride != 0)AI_Alive_Civilian_Ref->FloatingSpeed = FloatingSpeedToOverride;

		AI_Alive_Ref = AI_Alive_Civilian_Ref;
	}
}

bool APawn_Alive_Civilian::RecieveDamage(float DamageToRecieve)
{
	if (IsValid(AIasDummy))
	{
		AIasDummy->DamageParentDisStructure(DamageToRecieve);
	}
	else
	{
		HP = HP - DamageToRecieve;
		if (HP <= 0)return true;
	}

	return false;
}

void APawn_Alive_Civilian::Kill()
{
	OverzombieController_Ref->AllCivilians.Remove(AI_Alive_Civilian_Ref);
	AI_Alive_Civilian_Ref->Destroy();

	Super::Kill();
}

void APawn_Alive_Civilian::OnBeginNoiseNSmell(AActor* ActorInRange)
{
	if (!IsValid(AI_Alive_Civilian_Ref))return;

	Super::OnBeginNoiseNSmell(ActorInRange);
}

void APawn_Alive_Civilian::OnEndNoiseNSmell(AActor* ActorOutOfRange)
{
	if (!IsValid(AI_Alive_Civilian_Ref))return;

	Super::OnEndNoiseNSmell(ActorOutOfRange);
}