// Fill out your copyright notice in the Description page of Project Settings.


#include "Duty.h"
#include "OverzombieController.h"
#include "TaskManagerComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AI_Duty.h"
#include "Engine/TargetPoint.h"

// Sets default values
ADuty::ADuty()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
}

// Called when the game starts or when spawned
void ADuty::BeginPlay()
{
	Super::BeginPlay();
	
	auto bufController = Cast<AOverzombieController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(bufController))TaskManagerComponent = bufController->TaskManagerComponent;

	InitializeSlots();
	InitializeAI();
}

// Called every frame
void ADuty::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp,Warning, TEXT("TASK MANAGER COMPONENT: %s"),*TaskManagerComponent->GetName())
}

// Called to bind functionality to input
void ADuty::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADuty::InitializeSlots()
{
	ATargetPoint* bufCurrentTarget;

	TArray<UActorComponent*> SlotComponents = this->GetComponentsByClass(UChildActorComponent::StaticClass());
	for (int32 i = 0; i < SlotComponents.Num(); i++)
	{
		if (IsValid(SlotComponents[i]))
		{


			bufCurrentTarget = Cast<ATargetPoint>((Cast<UChildActorComponent>(SlotComponents[i]))->GetChildActor());
			if (IsValid(bufCurrentTarget))
			{
				DutySlots.Add(bufCurrentTarget);
			}
		}
	}
}

void ADuty::InitializeAI()
{
	ControllingAI = GetWorld()->SpawnActor<AAI_Duty>();
	if (IsValid(ControllingAI))
	{
		ControllingAI->Possess(this);
		ControllingAI->SetPossessedDuty(this);
		ControllingAI->SetOverwatchingTime(OverwatchingTime);
		//ControllingAI->InitializeSlots();

		MovementComponent->MaxSpeed = DutySpeed;
	}

	
}

TArray<ATargetPoint*> ADuty::GetDutySlots()
{
	return DutySlots;
}

TArray<ATargetPoint*> ADuty::GetPatrollingPoints()
{
	return PatrollingPoints;
}