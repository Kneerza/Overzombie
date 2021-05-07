// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Alive.h"
#include "Components/SphereComponent.h"
#include "Pawn_Zombie.h"
#include "AI_Zombie.h"
#include "AI_Overzombie_Alive.h"

APawn_Alive::APawn_Alive()
{


	// --- Hearing ---

	Hearing = CreateDefaultSubobject<USphereComponent>("Hearing");
	Hearing->SetupAttachment(Body);
	//Hearing->SetSphereRadius(HearingRadius);
	

	//CollisionProfile
	Hearing->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	Hearing->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Hearing->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	//---------------------------------------------------------------------------------------------------------
	Hearing->SetSphereRadius(HearingRadius);
	// ------
		

	// --- NoiseNSmell ---

	NoiseNSmell = CreateDefaultSubobject<USphereComponent>("Noise&Smell");
	NoiseNSmell->SetupAttachment(Body);
	//Hearing->SetSphereRadius(HearingRadius);


	//CollisionProfile
	NoiseNSmell->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	NoiseNSmell->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	NoiseNSmell->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	//---------------------------------------------------------------------------------------------------------
	NoiseNSmell->SetSphereRadius(NoiseNSmellRadius);
	// ------
}

// Called when the game starts or when spawned
void APawn_Alive::BeginPlay()
{
	Super::BeginPlay();

	Hearing->SetSphereRadius(HearingRadius);

	NoiseNSmell->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	NoiseNSmell->SetSphereRadius(NoiseNSmellRadius);

	auto StartLoc = GetActorLocation();
}

void APawn_Alive::BeginHearing(AActor* ActorInRange)
{
	auto ZombieInRange = Cast<APawn_Zombie>(ActorInRange);
	if ((ZombieInRange) && (AI_Alive_Ref)) {
		AI_Alive_Ref->AllZombies_Hearing.AddUnique(ZombieInRange);
	}
}

void APawn_Alive::EndHearing(AActor* ActorOutOfRange)
{
	auto ZombieOutOfRange = Cast<APawn_Zombie>(ActorOutOfRange);

	if ((ZombieOutOfRange) && (AI_Alive_Ref)) {
		AI_Alive_Ref->AllZombies_Hearing.Remove(ZombieOutOfRange);
	}
}

void APawn_Alive::OnBeginNoiseNSmell(AActor* ActorInRange)
{
	auto ZombieInRange = Cast<APawn_Zombie>(ActorInRange);
	if ((ZombieInRange)) {
		if (!IsValid(ZombieInRange))return;
		if (!IsValid(ZombieInRange->AI_Zombie_Ref))return;
		if (!IsValid(AI_Alive_Ref))return;
		AI_Alive_Ref->AllZombies_NoiseNSmell.Add(ZombieInRange);

		ZombieInRange->AI_Zombie_Ref->AllAlives_Hearing.Add(this);
		ZombieInRange->AI_Zombie_Ref->AliveToConsume = this;
	}
}

void APawn_Alive::OnEndNoiseNSmell(AActor* ActorOutOfRange)
{
	auto ZombieOutOfRange = Cast<APawn_Zombie>(ActorOutOfRange);

	if ((ZombieOutOfRange) && (AI_Alive_Ref)) {
		AI_Alive_Ref->AllZombies_NoiseNSmell.Remove(ZombieOutOfRange);

		ZombieOutOfRange->AI_Zombie_Ref->AllAlives_Hearing.Remove(this);

		if (!ZombieOutOfRange->AI_Zombie_Ref->isUnderControl)
			ZombieOutOfRange->AI_Zombie_Ref->ChangeAliveToConsume();
		else ZombieOutOfRange->AI_Zombie_Ref->AliveToConsume = NULL;
	}
}

void APawn_Alive::InitializeAI()
{

}

// Returns true if HP is < 0
bool APawn_Alive::RecieveDamage(float DamageToRecieve)
{
	HP -= DamageToRecieve;

	if (HP <= 0)return true;

	return false;
}