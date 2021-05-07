// Fill out your copyright notice in the Description page of Project Settings.


#include "PsychoSphere.h"
#include "Components/SphereComponent.h"
#include "Pawn_Zombie.h"
#include "AI_Zombie.h"

// Sets default values
APsychoSphere::APsychoSphere()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	RootComponent = CollisionSphere;
}

// Called when the game starts or when spawned
void APsychoSphere::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->SetSphereRadius(Radius);
}

// Called every frame
void APsychoSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APsychoSphere::OnBeginOverlap(AActor* OverlappingActor)
{
	UE_LOG(LogTemp, Error, TEXT("BeginOverlapping: %s"), *OverlappingActor->GetName())

		auto ZombieAIToAdd = Cast<APawn_Zombie>(OverlappingActor)->AI_Zombie_Ref;

		ControlledZombies.AddUnique(ZombieAIToAdd);
		ZombieAIToAdd->isUnderControl = true;
		ZombieAIToAdd->ChangeBehavior(EBehaviorMode::Purpose);

}

void APsychoSphere::OnEndOverlap(AActor* OverlappingActor)
{
	//UE_LOG(LogTemp, Error, TEXT("EndOverlapping: %s"), *OverlappingActor->GetName())
}

