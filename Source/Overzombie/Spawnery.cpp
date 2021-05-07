// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawnery.h"
#include "OverzombieController.h"
#include "Pawn_Overzombie.h"

// Sets default values
ASpawnery::ASpawnery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnery::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController_Ref = Cast<AOverzombieController>(GetWorld()->GetFirstPlayerController());

	PlayerController_Ref->Spawnaries.AddUnique(this);

	OriginalRechargeTime = RechargeTime;
}

// Called every frame
void ASpawnery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((GetWorld()->GetTimeSeconds() - LastSpawnTime) >= RechargeTime)
	{
		Spawn();
	}
}

void ASpawnery::Spawn()
{
	if (isMilitary) {
		if (PlayerController_Ref->CurrentMaxSoldiersCount - PlayerController_Ref->AllSoldiers.Num() <= 0) return;
	}
	else
	{
		if (PlayerController_Ref->CurrentMaxWorkersCount - PlayerController_Ref->AllWorkers.Num() <= 0) return;
	}

	FTransform SpawnTransform = GetActorTransform();

	GetWorld()->SpawnActor<APawn_Overzombie>(PawnClassToSpawn, SpawnTransform);

	LastSpawnTime = GetWorld()->GetTimeSeconds();
}
