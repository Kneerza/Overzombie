// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawnery.h"
#include "DrawDebugHelpers.h"
#include "AI_Zombie.h"
#include "OverzombieController.h"
#include "Pawn_Zombie.h"

void AZombieSpawnery::BeginPlay()
{
	Super::BeginPlay();

}

void AZombieSpawnery::Spawn()
{
	if (isInactive)return;
	if (PlayerController_Ref->CurrentMaxZombiesCount - PlayerController_Ref->AllZombies <= 0) return;

	FTransform SpawnTransform = GetActorTransform();

	auto SpawnedZombie = GetWorld()->SpawnActor<APawn_Zombie>(PawnClassToSpawn, SpawnTransform);

	auto RandLength = FMath::RandRange(3.f, 400.f);
	auto RandAngle = FMath::RandRange((-1)*RandAngleRange, RandAngleRange);
	auto RallyPoint = GetActorLocation() + FRotator(0.f, RandAngle, 0.f).RotateVector(GetActorForwardVector()* (RallyPointOffset + RandLength));


	// --- Defining Z ---

	FHitResult HitResult;
	FVector bufOffset = FVector(10.f, 10.f, 10.f);
	ECollisionChannel Collision = ECollisionChannel::ECC_WorldStatic;
	FCollisionQueryParams Params;

	if (!ActorLineTraceSingle(HitResult, RallyPoint + bufOffset, RallyPoint - bufOffset, Collision, Params))
	{
		float bufNewElevation = 0.f;

		if (PlayerController_Ref->GetNextLowerFloorElevation(RallyPoint.Z, bufNewElevation))
		{
			RallyPoint.Z = bufNewElevation;
		}
	}
	// ------


	if (IsValid(SpawnedZombie))
	{
		if (areZombiesCharging)
		{
			RallyPoint = GetActorLocation() + FRotator(0.f, RandAngle, 0.f).RotateVector(GetActorForwardVector()* (RallyPointOffset)*300.f);
			SpawnedZombie->AI_Zombie_Ref->TurnToLocation(RallyPoint);
			SpawnedZombie->AI_Zombie_Ref->ChangeBehavior(EBehaviorMode::ChargeForward);
			SpawnedZombie->AI_Zombie_Ref->ChargingTime = ChargingTime;
		}
		else
		{
			SpawnedZombie->AI_Zombie_Ref->FloatToLocation(RallyPoint);
		}
	}
	LastSpawnTime = GetWorld()->GetTimeSeconds();

	if (isModifyingRechargeTimeAfterEachSpawn)
	{
		RechargeTime = OriginalRechargeTime + FMath::RandRange((-1)*OriginalRechargeTime*0.2f, OriginalRechargeTime*0.2f);
	}

	if (isOneCharge) Destroy();
}