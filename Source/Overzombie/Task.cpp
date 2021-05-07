// Fill out your copyright notice in the Description page of Project Settings.


#include "Task.h"
#include "Pawn_Zombie.h"
#include "SlotComponent.h"
#include "TaskManagerComponent.h"
#include "OverzombieController.h"
#include "AI_Alive_Soldier.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ATask::ATask()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestructionArea = CreateDefaultSubobject<USphereComponent>("DestructionArea");
	ConstructionArea = CreateDefaultSubobject<UBoxComponent>("ConstructionArea");
}

void ATask::Initialize_BP()
{
	ConstructionArea->AttachToComponent(DestructionArea, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATask::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Error, TEXT("Task %s BEGINS PLAY"), *GetName())

	DestructionArea->OnComponentBeginOverlap.AddDynamic(this, &ATask::OnZombieEnterDestructionArea);
	DestructionArea->OnComponentEndOverlap.AddDynamic(this, &ATask::OnZombieLeaveDestructionArea);

	ConstructionArea->OnComponentBeginOverlap.AddDynamic(this, &ATask::OnWorkerEnterConstructionArea);
	ConstructionArea->OnComponentEndOverlap.AddDynamic(this, &ATask::OnWorkerLeaveConstructionArea);

	CurrentHP = HP;
}

void ATask::InitializeTask(UTaskManagerComponent* TaskManagerComponentRef)
{
	TaskManagerComponent = TaskManagerComponentRef;
	TaskManagerComponent->MakeTaskAvailable(this, Priority);

	if(Priority>=6)
	TaskManagerComponent->GameOverCount++;
}

// Called every frame
void ATask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isGettingDamageByZombies) {
		CurrentHP -= ZombiesCount;
		if (CurrentHP <= 0 && isComplete)
		{
			isComplete = false;
			TaskProgress = 0;

			TaskManagerComponent->MakeTaskAvailable(this, Priority);
			AvailableSlots = AllSlots;
			isAvailable = true;

			TaskManagerComponent->PullHigherPriorityTaskSoldiersToLowerPriority(Priority, Capacity);

			OnDestructed();
		}
	}

	/*
	TaskProgress += CurrentWorkers;

	if (TaskProgress >= TaskDifficulty)
	{
		isComplete = true;
		OnComplete();

		if (isAvailable)
		{
			TaskManagerComponent->MakeTaskUnavailable(this);
			isAvailable = false;
		}

		if (Priority >= 6)
		{
			TaskManagerComponent->CompleteGameOverTask();
		}

		if (CurrentlyAssignedSoldiers.Num() > 0)
		{
			for (int32 i = 0; i < CurrentlyAssignedSoldiers.Num(); i++)
			{
				CurrentlyAssignedSoldiers[i]->SetCurrentTask(NULL);
				CurrentlyAssignedSoldiers[i]->SetCurrentTaskSlot(NULL);
			}

			CurrentlyAssignedSoldiers.Empty();
		}

		TaskProgress = 0;
		CurrentHP = HP;
	}*/
}

void ATask::OnZombieEnterDestructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ZombiesCount++;
	ZombiesEntered++;

	if (isKillingEnteringZombies)
	{
		auto AsZombie = Cast<APawn_Zombie>(OtherActor);
		if(IsValid(AsZombie))AsZombie->Kill();
	}

	if (isEndGameTask && (ZombiesEntered >= ZombiesToEnterForDestruction))
	{
		Cast<AOverzombieController>(GetWorld()->GetFirstPlayerController())->GameOver(true);
	}

	OnZombieEntered();
}

void ATask::OnZombieLeaveDestructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ZombiesCount--;
}

USlotComponent* ATask::AssignSoldier(AAI_Alive_Soldier* SoldierToAssign)
{
	CurrentlyAssignedSoldiers.AddUnique(SoldierToAssign);

	auto AssignedSlot = GetAvailableSlot();

	if (IsValid(AssignedSlot)) {
		SoldierToAssign->SetCurrentTaskSlot(AssignedSlot);

		AvailableSlots.Remove(AssignedSlot);
	}
	return AssignedSlot;
}

void ATask::DismissSoldier(AAI_Alive_Soldier* SoldierToDismiss)
{
	if (!IsValid(SoldierToDismiss))return;
	if (IsValid(SoldierToDismiss))if(CurrentlyAssignedSoldiers.Contains(SoldierToDismiss))CurrentlyAssignedSoldiers.Remove(SoldierToDismiss);
	AvailableSlots.AddUnique(SoldierToDismiss->GetCurrentTaskSlot());

	SoldierToDismiss->SetCurrentTask(NULL);
	SoldierToDismiss->SetCurrentTaskSlot(NULL);

	if (!isAvailable)
	{
		isAvailable = true;
		TaskManagerComponent->MakeTaskAvailable(this, Priority);/// AvailableTasks_0.AddUnique(this);
	}
}

int32 ATask::GetCapacity()
{
	return Capacity;
}

void ATask::InitializeSlots() 
{
	auto AllSlotsAsActorComponents = this->GetComponentsByClass(USlotComponent::StaticClass());

	if (AllSlotsAsActorComponents.Num() > 0)
	{
		for (int32 i = 0; i < AllSlotsAsActorComponents.Num(); i++)
		{
			AllSlots.AddUnique(Cast<USlotComponent>(AllSlotsAsActorComponents[i]));
		}
		AvailableSlots = AllSlots;
	}

	Capacity = AllSlots.Num();
	//for (int32 i = 0; i < SlotComponents.Num(); i++)
//	{
	//	if (IsValid(SlotComponents[i]))
		//{


			//bufCurrentTarget = Cast<ATargetPoint>((Cast<UChildActorComponent>(SlotComponents[i]))->GetChildActor());
			//if (IsValid(bufCurrentTarget))
			//{
			//	DutySlots.Add(bufCurrentTarget);
			//}
		//}
	//}
}

USlotComponent* ATask::GetAvailableSlot()
{
	if (AvailableSlots.Num() == 0)return NULL;
	return AvailableSlots[0];
}


void ATask::OnWorkerEnterConstructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//CurrentWorkers++;
}

void ATask::OnWorkerLeaveConstructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//CurrentWorkers--;
}

// Returns true if task is completed
bool ATask::AddTaskProgress(float AmountToAdd)
{
	TaskProgress += AmountToAdd;

	bool isTaskCompleted = false;

	if (TaskProgress >= TaskDifficulty)
	{
		isComplete = true;
		OnComplete();

		if (isAvailable)
		{
			TaskManagerComponent->MakeTaskUnavailable(this);
			isAvailable = false;
		}

		if (Priority >= 6)
		{
			
			TaskManagerComponent->CompleteGameOverTask();

		}
		
		TaskProgress = 0;
		CurrentHP = HP;
		isTaskCompleted = true;

	}

	UE_LOG(LogTemp, Warning, TEXT("Task:  %s       Progress: %f    of    %i"),*GetName(),TaskProgress, TaskDifficulty)

	return isTaskCompleted;
}
