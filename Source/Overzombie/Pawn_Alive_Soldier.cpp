// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Alive_Soldier.h"
#include "TaskManagerComponent.h"
#include "Task.h"
#include "OverzombieController.h"
#include "AI_Alive_Soldier.h"
#include "Duty.h"
#include "AI_Duty.h"
#include "Pawn_Zombie.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


APawn_Alive_Soldier::APawn_Alive_Soldier()
{
	FireSocket = CreateDefaultSubobject<UStaticMeshComponent>("FireSocket");
	FireSocket->SetupAttachment(Body);
	FireSocket->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APawn_Alive_Soldier::Initialize_BP()
{
	Super::Initialize_BP();
}

void APawn_Alive_Soldier::BeginPlay()
{
	Super::BeginPlay();

	// ...

	InitializeAI();
}

void APawn_Alive_Soldier::BeginHearing(AActor* ActorInRange)
{
	Super::BeginHearing(ActorInRange);

}

void APawn_Alive_Soldier::EndHearing(AActor* ActorOutOfRange)
{
	Super::EndHearing(ActorOutOfRange);

}

void APawn_Alive_Soldier::InitializeAI()
{
	
	if (AI_Possessing_Class) {

		AI_Alive_Soldier_Ref = Cast<AAI_Alive_Soldier>(GetWorld()->SpawnActor(AI_Possessing_Class));
		
	}
	
	if (AI_Alive_Soldier_Ref)
	{
		AI_Alive_Soldier_Ref->Possess(this);
		AI_Alive_Soldier_Ref->PossessedAliveSoldier_Ref = this;

		
		AI_Alive_Soldier_Ref->SetupWeapon(ReloadingSpeed, Damage, isSquareDamage, WeaponNoiseRadius, AimingTime);
		AI_Alive_Soldier_Ref->SetIsAvailableForDuty(isAvailableForDuty);
		AI_Alive_Soldier_Ref->ActivateAI();

		AI_Alive_Ref = AI_Alive_Soldier_Ref;

		AI_Alive_Soldier_Ref->FireSocket = FireSocket;
	}
	
}

void APawn_Alive_Soldier::Kill()
{
	if (IsValid(AI_Alive_Soldier_Ref)) {
		auto CurrentDuty = AI_Alive_Soldier_Ref->GetCurrentDuty();
		if (IsValid(CurrentDuty)) {
			auto Duty_AI = CurrentDuty->ControllingAI;
			if (IsValid(Duty_AI))		// TODO Move some AI functionality to AI instead of pawn
			{
				Duty_AI->RemoveEnlistedSoldier(AI_Alive_Soldier_Ref);
			}
		}
		AI_Alive_Soldier_Ref->TaskManagerComponent->RemoveFromAvailableSoldiers(AI_Alive_Soldier_Ref);
		OverzombieController_Ref->AllSoldiers.Remove(AI_Alive_Soldier_Ref);
		if (IsValid(AI_Alive_Soldier_Ref))if(IsValid(AI_Alive_Soldier_Ref->GetCurrentTask()))
		AI_Alive_Soldier_Ref->GetCurrentTask()->DismissSoldier(AI_Alive_Soldier_Ref);
		if (IsValid(AI_Alive_Soldier_Ref))AI_Alive_Soldier_Ref->ReleaseTask();
		if(IsValid(AI_Alive_Soldier_Ref))AI_Alive_Soldier_Ref->Destroy();
		UE_LOG(LogTemp, Error, TEXT("ATTN:                   AI IS DESTROYED"))
	}

	Super::Kill();
}

void APawn_Alive_Soldier::OnBeginNoiseNSmell(AActor* ActorInRange)
{
	if (!IsValid(AI_Alive_Soldier_Ref))return;



	Super::OnBeginNoiseNSmell(ActorInRange);
}

void APawn_Alive_Soldier::OnEndNoiseNSmell(AActor* ActorOutOfRange)
{
	if (!IsValid(AI_Alive_Soldier_Ref))return;

	Super::OnEndNoiseNSmell(ActorOutOfRange);
}