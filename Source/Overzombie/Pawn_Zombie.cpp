// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Zombie.h"
#include "Components/CapsuleComponent.h"
#include "PsychoSphere.h"
//#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "OverzombieController.h"
#include "AI_Overzombie.h"
#include "AI_Zombie.h"

APawn_Zombie::APawn_Zombie()
{
	
	//OnActorBeginOverlap.AddDynamic(UPrimitiveComponent* overlappedComp, &APawn_Zombie::OnBeginOverlap);
}

void APawn_Zombie::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &APawn_Zombie::OnBeginOverlap);
		if (AI_Possessing_Class) {

			AI_Zombie_Ref = Cast<AAI_Zombie>(GetWorld()->SpawnActor(AI_Possessing_Class));
		}

	if (AI_Zombie_Ref)
	{
		AI_Zombie_Ref->Possess(this);
		AI_Zombie_Ref->PossessedZombie_Ref = this;
		///AI_Zombie_Ref->PointOfInterest = PointOfInterest;
		AI_Zombie_Ref->ActivateAI();

		//AI_Zombie_Ref->FloatToLocation(RallyPoint);
	}

	OverzombieController_Ref->AllZombies++;
	PsychoSphere = OverzombieController_Ref->PsychoSphere;
}

void APawn_Zombie::Kill()
{
	OverzombieController_Ref->AllZombies--;
	OverzombieController_Ref->NEW_AllZombies.Remove(AI_Zombie_Ref);
	AI_Zombie_Ref->Destroy();
	OverzombieController_Ref->CheckSpawneries();

	Super::Kill();
}

void APawn_Zombie::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Error, TEXT("HERE: %s"),*OtherActor->GetName())
		
	if (OtherActor == PsychoSphere)
	{
		PsychoSphere->ControlledZombies.AddUnique(AI_Zombie_Ref);
		AI_Zombie_Ref->isUnderControl = true;
		AI_Zombie_Ref->ChangeBehavior(EBehaviorMode::Purpose);
	}
	
}

void APawn_Zombie::SetIsFalling(bool NewIsFalling)
{
	isFalling = NewIsFalling;
	if (IsValid(AI_Zombie_Ref))AI_Zombie_Ref->SetIsFalling(NewIsFalling);// isFalling = NewIsFalling;
}

bool APawn_Zombie::GetIsFalling()
{
	return AI_Zombie_Ref->GetIsFalling();//isFalling;
}