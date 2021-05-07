// Fill out your copyright notice in the Description page of Project Settings.


#include "OverzombieController.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "OZWidget.h"
#include "PsychoSphere.h"
#include "Components/SphereComponent.h"
#include "AI_Zombie.h"
#include "Engine/TargetPoint.h"
#include "Spawnery.h"
#include "Pawn_Alive.h"
#include "Pawn_Zombie.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "TaskManagerComponent.h"

AOverzombieController::AOverzombieController()
{
	PrimaryActorTick.bCanEverTick = true;

	TaskManagerComponent = CreateDefaultSubobject<UTaskManagerComponent>("TaskManagerComponent");
}

void AOverzombieController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, HitResult))
	{
		CurrentCursorLocation = HitResult.Location;
		PsychoSphere->SetActorLocation(CurrentCursorLocation);
	}

	if (CurrentTarget) {
		CurrentTarget->SetActorLocation(CurrentCursorLocation);
	}
}

void AOverzombieController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PsychoSphere = GetWorld()->SpawnActor<APsychoSphere>(PsychoSphereClassToSpawn);
	PsychoSphere->SetActorEnableCollision(false);

	G_HuddleOffsetFromPointOfInterestMIN = MinSphereRadius;
	G_HuddleOffsetFromPointOfInterestMAX = MaxSphereRadius;

	FindAllFloorElevations();
}

void AOverzombieController::SetupInputComponent()
{
	Super::SetupInputComponent();


	// --- Action ---

	InputComponent->BindAction(AOverzombieController::LargeAreaInteraction, IE_Pressed, this, &AOverzombieController::LargeAreaInteraction_Press_Handle);
	InputComponent->BindAction(AOverzombieController::LargeAreaInteraction, IE_Released, this, &AOverzombieController::LargeAreaInteraction_Release_Handle);
	InputComponent->BindAction(AOverzombieController::SmallAreaInteraction, IE_Pressed, this, &AOverzombieController::SmallAreaInteraction_Press_Handle);
	InputComponent->BindAction(AOverzombieController::SmallAreaInteraction, IE_Released, this, &AOverzombieController::SmallAreaInteraction_Release_Handle);

}

void AOverzombieController::Effect_Shoot(EShootEffectType ShootEffectType, FTransform Transform)
{
	UParticleSystem* SystemToSpawn = NULL;

	switch (ShootEffectType)
	{
	case EShootEffectType::Pistol:

		if (!EffectSoldierPistolShootClassToSpawn)return;

		SystemToSpawn = EffectSoldierPistolShootClassToSpawn;

		break;

	case EShootEffectType::AR:

		if (!EffectSoldierARShootClassToSpawn)return;

		SystemToSpawn = EffectSoldierARShootClassToSpawn;

		break;
	}

	if (!SystemToSpawn)return;
	else UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SystemToSpawn, Transform);
	UE_LOG(LogTemp, Error, TEXT("----------------------- SHOOOOOOOTING!!!!!!!!!!!"))

}

void AOverzombieController::Effect_Bleed(FVector Location)
{
	if (!EffectBleedClassToSpawn)return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EffectBleedClassToSpawn, Location);
}

void AOverzombieController::LargeAreaInteraction_Press_Handle()
{
	isLBPressed = true;

	if (isRBPressed)
	{
		G_HuddleOffsetFromPointOfInterest = G_HuddleOffsetFromPointOfInterestMAX;

		auto bufZombies = PsychoSphere->ControlledZombies;
		if (bufZombies.Num() > 0)
		{
			for (int32 i = 0; i < bufZombies.Num(); i++)
			{
				bufZombies[i]->ChangeBehavior(EBehaviorMode::Purpose);
			}
		}

		PsychoSphere->CollisionSphere->SetSphereRadius(MaxSphereRadius);
	}
	else {
		G_HuddleOffsetFromPointOfInterest = G_HuddleOffsetFromPointOfInterestMAX;

		PsychoSphere->CollisionSphere->SetSphereRadius(MaxSphereRadius);
		PsychoSphere->SetActorEnableCollision(true);
	}
}

void AOverzombieController::LargeAreaInteraction_Release_Handle()
{
	isLBPressed = false;

	if (isRBPressed)
	{
		G_HuddleOffsetFromPointOfInterest = G_HuddleOffsetFromPointOfInterestMIN;

		auto bufZombies = PsychoSphere->ControlledZombies;
		if (bufZombies.Num() > 0)
		{
			for (int32 i = 0; i < bufZombies.Num(); i++)
			{
				bufZombies[i]->ChangeBehavior(EBehaviorMode::Purpose);
			}
		}

		PsychoSphere->CollisionSphere->SetSphereRadius(MinSphereRadius);

		return;
	}

	PsychoSphere->SetActorEnableCollision(false);
	
	auto ZombiesToRelease = PsychoSphere->ControlledZombies;
	PsychoSphere->ControlledZombies.Empty();

	if (ZombiesToRelease.Num() > 0)
	{
		for (int32 i = 0; i < ZombiesToRelease.Num(); i++)
		{
			if (IsValid(ZombiesToRelease[i]))
			{
				ZombiesToRelease[i]->isUnderControl = false;
			}
		}
	}
}

void AOverzombieController::SmallAreaInteraction_Press_Handle()
{
	isRBPressed = true;

	if (isLBPressed)
	{
		G_HuddleOffsetFromPointOfInterest = G_HuddleOffsetFromPointOfInterestMIN;

		auto bufZombies = PsychoSphere->ControlledZombies;
		if (bufZombies.Num() > 0)
		{
			for (int32 i = 0; i < bufZombies.Num(); i++)
			{
				bufZombies[i]->ChangeBehavior(EBehaviorMode::Purpose);
			}
		}

		PsychoSphere->CollisionSphere->SetSphereRadius(MinSphereRadius);
	}
	else {
		G_HuddleOffsetFromPointOfInterest = G_HuddleOffsetFromPointOfInterestMIN;

		PsychoSphere->CollisionSphere->SetSphereRadius(MinSphereRadius);
		PsychoSphere->SetActorEnableCollision(true);
	}
}

void AOverzombieController::SmallAreaInteraction_Release_Handle()
{
	isRBPressed = false;

	if (isLBPressed)
	{
		G_HuddleOffsetFromPointOfInterest = G_HuddleOffsetFromPointOfInterestMAX;

		auto bufZombies = PsychoSphere->ControlledZombies;
		if (bufZombies.Num() > 0)
		{
			for (int32 i = 0; i < bufZombies.Num(); i++)
			{
				bufZombies[i]->ChangeBehavior(EBehaviorMode::Purpose);
			}
		}

		PsychoSphere->CollisionSphere->SetSphereRadius(MaxSphereRadius);

		return;
	}

	PsychoSphere->SetActorEnableCollision(false);

	auto ZombiesToRelease = PsychoSphere->ControlledZombies;
	PsychoSphere->ControlledZombies.Empty();

	if (ZombiesToRelease.Num() > 0)
	{
		for (int32 i = 0; i < ZombiesToRelease.Num(); i++)
		{
			if (IsValid(ZombiesToRelease[i]))
			{
				ZombiesToRelease[i]->isUnderControl = false;
			}
		}
	}
}

void AOverzombieController::TurnAliveIntoZombie(APawn_Alive* AliveToTurn)
{
	if (!ZombieBPClassToSpawn) { UE_LOG(LogTemp, Warning, TEXT("Controller: No zombie class set")) return; }
	if (!AliveToTurn) { return; }
	if (AliveToTurn->isDummy)return;
	auto AliveLocation = AliveToTurn->GetActorLocation();
	AliveToTurn->Kill();
	auto Zombie = GetWorld()->SpawnActor<APawn_Zombie>(ZombieBPClassToSpawn);
	Zombie->SetActorLocation(AliveLocation);
	Zombie->AI_Zombie_Ref->Elevation = GetClosestFloorElevation(Zombie->GetActorLocation().Z) + Zombie->AI_Zombie_Ref->ZombiePivotOffset;
}

void AOverzombieController::SpawnSoldiers()
{
	//auto AvailableSoldiersCount = CurrentMaxSoldiersCount - AllSoldiers.Num();
	//if ((Spawnaries.Num() > 0) && (AvailableSoldiersCount>0))
//	{
	//	for (int32 i = 0; i < Spawnaries.Num(); i++)
		//{
		//	Spawnaries[i]->WaitingToSpawnCounts++;
		//}
	//}
}

void AOverzombieController::GameOver(bool isWin)
{
	auto EndGameScreen = CreateWidget<UOZWidget>(this, EndGameScreenClassToSpawn);
	EndGameScreen->OnInitializeEndGame(isWin);
	EndGameScreen->AddToViewport(9999);
}

void AOverzombieController::FindAllFloorElevations()
{
	TArray<AActor*> FoundFloors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass(), FoundFloors);

	for (AActor* Floor : FoundFloors)
	{
			FloorElevations.AddUnique(Floor->GetActorLocation().Z);
	}
}

float AOverzombieController::GetClosestFloorElevation(float Elevation)
{
	float ClosestElevation = 9000;
	float MinDifference = 9000;

	for (float bufElevation : FloorElevations)
	{
		if (FMath::Abs(Elevation - bufElevation) < MinDifference)
		{
			MinDifference = FMath::Abs(Elevation - bufElevation);
			ClosestElevation = bufElevation;
		}
	}

	return ClosestElevation;
}

bool AOverzombieController::GetNextLowerFloorElevation(float Elevation, float &OUT_LowerElevation)
{
	OUT_LowerElevation = 9000;
	float MinDifference = 9000;

	for (float bufElevation : FloorElevations)
	{
		if (bufElevation >= Elevation)continue;
		if (FMath::Abs(Elevation - bufElevation) < MinDifference)
		{
			MinDifference = FMath::Abs(Elevation - bufElevation);
			OUT_LowerElevation = bufElevation;
		}
	}

	if (OUT_LowerElevation == 9000)return false;
	return true;
}

FName AOverzombieController::LargeAreaInteraction = FName(TEXT("LargeAreaInteraction"));
FName AOverzombieController::SmallAreaInteraction = FName(TEXT("SmallAreaInteraction"));

FName AOverzombieController::Zoom = FName(TEXT("Zoom"));
FName AOverzombieController::MoveLeftRight = FName(TEXT("MoveLeftRight"));
FName AOverzombieController::MoveUpDown = FName(TEXT("MoveUpDown"));