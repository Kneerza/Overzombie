// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleTaskSlot.h"
#include "Task.h"
#include "AI_Alive_Soldier.h"
#include "AI_Alive_Worker.h"
#include "OverzombieController.h"
#include "TaskManagerComponent.h"
#include "Components/BoxComponent.h"
#include "Pawn_Alive_Soldier.h"
#include "Pawn_Alive_Worker.h"

// Sets default values
ASingleTaskSlot::ASingleTaskSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("BoxCollisionComponent");
	RootComponent = BoxCollisionComponent;
}

// Called when the game starts or when spawned
void ASingleTaskSlot::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASingleTaskSlot::OnBeginOverlap);

	TaskManagerComponent = Cast<AOverzombieController>(GetWorld()->GetFirstPlayerController())->TaskManagerComponent;
	

	auto Parent = Cast<ATask>(GetParentActor());

	if (IsValid(Parent))
	{
		OwnerTask = Parent;
		Priority = OwnerTask->Priority;
	}

	MakeAvailable();
}

// Called every frame
void ASingleTaskSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASingleTaskSlot::Delete()
{
	if (isMilitary) {
		TaskManagerComponent->RemoveFromAvailableSingleTaskSlots(this);
	}
	else
	{
		TaskManagerComponent->RemoveFromAvailableSingleWorkerTaskSlots(this);
	}
	Destroy();
}

void ASingleTaskSlot::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("--------------------------- COLLIDING WITH TASK ------------------------------"))
	if (isMilitary) {
		if (!IsValid(AssignedSoldier))return;
		if (OtherActor == AssignedSoldier->PossessedAliveSoldier_Ref)
		{
			if (isTaskInfinite)AssignedSoldier->ChangeBehavior(EBehaviorMode::WorkingOnInfiniteTask);
			else AssignedSoldier->ChangeBehavior(EBehaviorMode::WorkingOnTask);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("--------------------------- COLLIDING WITH TASK ------------------------------"))
		if (!IsValid(AssignedWorker))return;
		if (OtherActor == AssignedWorker->PossessedAliveWorker_Ref)
		{
			if (isTaskInfinite)AssignedWorker->ChangeBehavior(EBehaviorMode::WorkingOnInfiniteTask);
			else AssignedWorker->ChangeBehavior(EBehaviorMode::WorkingOnTask);
		}
	}
}

// Returns true if task is completed
bool ASingleTaskSlot::AddTaskProgress(float AmountToAdd)
{
	if (!IsValid(OwnerTask))return false;

	return OwnerTask->AddTaskProgress(AmountToAdd);
}

void ASingleTaskSlot::Kill()
{
	if (isMilitary)
	{
		TaskManagerComponent->RemoveFromAvailableSingleTaskSlots(this);
	}
	else
	{
		TaskManagerComponent->RemoveFromAvailableSingleWorkerTaskSlots(this);
	}

	if (IsValid(AssignedSoldier))
	{
		AssignedSoldier->CurrentSingleTask = NULL;
		AssignedSoldier->ChangeBehavior(EBehaviorMode::Idle);
		TaskManagerComponent->AddToAvailableSoldiers(AssignedSoldier);
	}

	if (IsValid(AssignedWorker))
	{
		AssignedWorker->CurrentSingleTask = NULL;
		AssignedWorker->ChangeBehavior(EBehaviorMode::Idle);
		TaskManagerComponent->AddToAvailableWorkers(AssignedWorker);
	}
	
	Destroy();
}

void ASingleTaskSlot::MakeAvailable()
{
	AssignedSoldier = NULL;
	AssignedWorker = NULL;

	if (isMilitary) {
		TaskManagerComponent->AddToAvailableSingleTaskSlots(this);
	}
	else
	{
		TaskManagerComponent->AddToAvailableSingleWorkerTaskSlots(this);
	}
}

int32 ASingleTaskSlot::GetPriority()
{
	return Priority;
}

void ASingleTaskSlot::SetPriority(int32 NewPriority)
{
	Priority = NewPriority;
}