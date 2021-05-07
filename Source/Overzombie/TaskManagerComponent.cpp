// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskManagerComponent.h"
#include "AI_Alive_Soldier.h"
#include "AI_Alive_Worker.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OverzombieController.h"
#include "SingleTaskSlot.h"
#include "Task.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTaskManagerComponent::UTaskManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTaskManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	//UE_LOG(LogTemp, Error, TEXT("MANAGERCOMPONENT BEGINS PLAY"), *GetName())

	TArray<AActor*> AllTasksAsActors;
	ATask* AsTask;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATask::StaticClass(), AllTasksAsActors);

	for (int32 i = 0; i < AllTasksAsActors.Num(); i++)
	{
		AsTask = Cast<ATask>(AllTasksAsActors[i]);
		if (IsValid(AsTask))AsTask->InitializeTask(this);
	}
}


// Called every frame
void UTaskManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Returns Task soldier was assigned to
void UTaskManagerComponent::AssignSoldier(AAI_Alive_Soldier* SoldierToAssign)
{
	/*
	auto bufTask = GetFirstPriorityTask();
	if (!IsValid(bufTask))return;

	auto bufAssignedSlot = bufTask->AssignSoldier(SoldierToAssign);
	SoldierToAssign->SetCurrentTask(bufTask);
	//SoldierToAssign->SetCurrentTaskSlot(bufAssignedSlot);

	if (bufTask->CurrentlyAssignedSoldiers.Num() >= bufTask->GetCapacity())
	{
		bufTask->isAvailable = false;
		MakeTaskUnavailable(bufTask);
	}
	*/
}

ATask* UTaskManagerComponent::GetFirstPriorityTask()
{
	if (AvailableTasks_0.Num() > 0)return AvailableTasks_0[FMath::RandRange(0, AvailableTasks_0.Num()-1)];
	if (AvailableTasks_1.Num() > 0)return AvailableTasks_1[FMath::RandRange(0, AvailableTasks_1.Num()-1)];
	if (AvailableTasks_2.Num() > 0)return AvailableTasks_2[FMath::RandRange(0, AvailableTasks_2.Num()-1)];
	if (AvailableTasks_3.Num() > 0)return AvailableTasks_3[FMath::RandRange(0, AvailableTasks_3.Num()-1)];
	if (AvailableTasks_4.Num() > 0)return AvailableTasks_4[FMath::RandRange(0, AvailableTasks_4.Num()-1)];
	if (AvailableTasks_5.Num() > 0)return AvailableTasks_5[FMath::RandRange(0, AvailableTasks_5.Num()-1)];
	if (AvailableTasks_6.Num() > 0)return AvailableTasks_6[FMath::RandRange(0, AvailableTasks_6.Num()-1)];
	
	return NULL;
}

void UTaskManagerComponent::MakeTaskUnavailable(ATask* TaskToMakeUnavailable)
{
	if (AvailableTasks_0.Contains(TaskToMakeUnavailable)) { AvailableTasks_0.Remove(TaskToMakeUnavailable); return; }
	if (AvailableTasks_1.Contains(TaskToMakeUnavailable)) { AvailableTasks_1.Remove(TaskToMakeUnavailable); return; }
	if (AvailableTasks_2.Contains(TaskToMakeUnavailable)) { AvailableTasks_2.Remove(TaskToMakeUnavailable); return; }
	if (AvailableTasks_3.Contains(TaskToMakeUnavailable)) { AvailableTasks_3.Remove(TaskToMakeUnavailable); return; }
	if (AvailableTasks_4.Contains(TaskToMakeUnavailable)) { AvailableTasks_4.Remove(TaskToMakeUnavailable); return; }
	if (AvailableTasks_5.Contains(TaskToMakeUnavailable)) { AvailableTasks_5.Remove(TaskToMakeUnavailable); return; }
	if (AvailableTasks_6.Contains(TaskToMakeUnavailable)) { AvailableTasks_6.Remove(TaskToMakeUnavailable); return; }

	
}

void UTaskManagerComponent::MakeTaskAvailable(ATask* TaskToMakeAvailable, int32 Priority)
{
	switch (Priority)
	{
	case 0:
		AvailableTasks_0.AddUnique(TaskToMakeAvailable);
		break;

	case 1:
		AvailableTasks_1.AddUnique(TaskToMakeAvailable);
		break;

	case 2:
		AvailableTasks_2.AddUnique(TaskToMakeAvailable);
		break;

	case 3:
		AvailableTasks_3.AddUnique(TaskToMakeAvailable);
		break;

	case 4:
		AvailableTasks_4.AddUnique(TaskToMakeAvailable);
		break;

	case 5:
		AvailableTasks_5.AddUnique(TaskToMakeAvailable);
		break;

	case 6:
		AvailableTasks_6.AddUnique(TaskToMakeAvailable);
		break;

	default:
		AvailableTasks_6.AddUnique(TaskToMakeAvailable);
		break;
	}
}

void UTaskManagerComponent::PullHigherPriorityTaskSoldiersToLowerPriority(int32 PriorityPullTo, int32 NumberOfSoldiersToPull)
{
	

	TArray<AAI_Alive_Soldier*> CurrentSoldiers;
	TArray<ATask*> CurrentlyCheckedTasks;

	switch (PriorityPullTo)
	{
	case 0:
		
		CurrentlyCheckedTasks = AllTasks_6;
		CurrentlyCheckedTasks.Append(AllTasks_5);
		CurrentlyCheckedTasks.Append(AllTasks_4);
		CurrentlyCheckedTasks.Append(AllTasks_3);
		CurrentlyCheckedTasks.Append(AllTasks_2);
		CurrentlyCheckedTasks.Append(AllTasks_1);

		break;

	case 1:

		CurrentlyCheckedTasks = AllTasks_6;
		CurrentlyCheckedTasks.Append(AllTasks_5);
		CurrentlyCheckedTasks.Append(AllTasks_4);
		CurrentlyCheckedTasks.Append(AllTasks_3);
		CurrentlyCheckedTasks.Append(AllTasks_2);
		
		break;

	case 2:

		CurrentlyCheckedTasks = AllTasks_6;
		CurrentlyCheckedTasks.Append(AllTasks_5);
		CurrentlyCheckedTasks.Append(AllTasks_4);
		CurrentlyCheckedTasks.Append(AllTasks_3);

		break;

	case 3:

		CurrentlyCheckedTasks = AllTasks_6;
		CurrentlyCheckedTasks.Append(AllTasks_5);
		CurrentlyCheckedTasks.Append(AllTasks_4);

		break;

	case 4:

		CurrentlyCheckedTasks = AllTasks_6;
		CurrentlyCheckedTasks.Append(AllTasks_5);

		break;

	case 5:

		CurrentlyCheckedTasks = AllTasks_6;
	
		break;

	default:
		return;
		break;
	}

	//UE_LOG(LogTemp, Error, TEXT("TASKS: %i"), CurrentlyCheckedTasks.Num())

	if (CurrentlyCheckedTasks.Num() > 0)
	{
		for (int32 i = 0; i < CurrentlyCheckedTasks.Num(); i++)
		{
			//UE_LOG(LogTemp, Error, TEXT("CurrentTask: %s"), *CurrentlyCheckedTasks[i]->GetName())
			CurrentSoldiers = CurrentlyCheckedTasks[i]->CurrentlyAssignedSoldiers;

			if (CurrentSoldiers.Num() > 0)
			{
				for (int32 j = CurrentSoldiers.Num() - 1; j >= 0; j--)
				{
					CurrentlyCheckedTasks[i]->DismissSoldier(CurrentSoldiers[j]);
					NumberOfSoldiersToPull--;

					//UE_LOG(LogTemp, Error, TEXT("CurrentSoldier: %s"),*CurrentSoldiers[j]->GetName())

					if (NumberOfSoldiersToPull == 0)return;
				}
			}
		}
	}
}

void UTaskManagerComponent::CompleteGameOverTask()
{
	
	GameOverCount--;
	if (GameOverCount == 0)
		Cast<AOverzombieController>(GetWorld()->GetFirstPlayerController())->GameOver(false);
}

void UTaskManagerComponent::AddToAvailableSingleTaskSlots(ASingleTaskSlot* SingleTaskSlotToAdd)
{
	if (!IsValid(SingleTaskSlotToAdd))return;

	switch (SingleTaskSlotToAdd->GetPriority())
	{
	case 0: AvailableSingleTaskSlots_0.AddUnique(SingleTaskSlotToAdd); break;

	case 1: AvailableSingleTaskSlots_1.AddUnique(SingleTaskSlotToAdd); break;

	case 2: AvailableSingleTaskSlots_2.AddUnique(SingleTaskSlotToAdd); break;

	case 3: AvailableSingleTaskSlots_3.AddUnique(SingleTaskSlotToAdd); break;

	case 4: AvailableSingleTaskSlots_4.AddUnique(SingleTaskSlotToAdd); break;

	case 5: AvailableSingleTaskSlots_5.AddUnique(SingleTaskSlotToAdd); break;

	default: AvailableSingleTaskSlots_6.AddUnique(SingleTaskSlotToAdd); break;
	}

	//AvailableSingleTaskSlots.AddUnique(SingleTaskSlotToAdd);
	AssignMilitaryTask();
}

void UTaskManagerComponent::RemoveFromAvailableSingleTaskSlots(ASingleTaskSlot* SingleTaskSlotToRemove)
{
	if (!IsValid(SingleTaskSlotToRemove))return;

	switch (SingleTaskSlotToRemove->GetPriority())
	{
	case 0: AvailableSingleTaskSlots_0.Remove(SingleTaskSlotToRemove); break;

	case 1: AvailableSingleTaskSlots_1.Remove(SingleTaskSlotToRemove); break;

	case 2: AvailableSingleTaskSlots_2.Remove(SingleTaskSlotToRemove); break;

	case 3: AvailableSingleTaskSlots_3.Remove(SingleTaskSlotToRemove); break;

	case 4: AvailableSingleTaskSlots_4.Remove(SingleTaskSlotToRemove); break;

	case 5: AvailableSingleTaskSlots_5.Remove(SingleTaskSlotToRemove); break;

	default: AvailableSingleTaskSlots_6.Remove(SingleTaskSlotToRemove); break;
	}

	//AvailableSingleTaskSlots.Remove(SingleTaskSlotToRemove);
}

void UTaskManagerComponent::AddToAvailableSingleWorkerTaskSlots(ASingleTaskSlot* SingleTaskSlotToAdd)
{
	if (!IsValid(SingleTaskSlotToAdd))return;

	switch (SingleTaskSlotToAdd->GetPriority())
	{
	case 0: AvailableSingleWorkerTaskSlots_0.AddUnique(SingleTaskSlotToAdd); break;

	case 1: AvailableSingleWorkerTaskSlots_1.AddUnique(SingleTaskSlotToAdd); break;

	case 2: AvailableSingleWorkerTaskSlots_2.AddUnique(SingleTaskSlotToAdd); break;

	case 3: AvailableSingleWorkerTaskSlots_3.AddUnique(SingleTaskSlotToAdd); break;

	case 4: AvailableSingleWorkerTaskSlots_4.AddUnique(SingleTaskSlotToAdd); break;

	case 5: AvailableSingleWorkerTaskSlots_5.AddUnique(SingleTaskSlotToAdd); break;

	default: AvailableSingleWorkerTaskSlots_6.AddUnique(SingleTaskSlotToAdd); break;
	}

	//AvailableSingleTaskSlots.AddUnique(SingleTaskSlotToAdd);
	AssignWorkerTask();
}

void UTaskManagerComponent::RemoveFromAvailableSingleWorkerTaskSlots(ASingleTaskSlot* SingleTaskSlotToRemove)
{
	if (!IsValid(SingleTaskSlotToRemove))return;

	switch (SingleTaskSlotToRemove->GetPriority())
	{
	case 0: AvailableSingleWorkerTaskSlots_0.Remove(SingleTaskSlotToRemove); break;

	case 1: AvailableSingleWorkerTaskSlots_1.Remove(SingleTaskSlotToRemove); break;

	case 2: AvailableSingleWorkerTaskSlots_2.Remove(SingleTaskSlotToRemove); break;

	case 3: AvailableSingleWorkerTaskSlots_3.Remove(SingleTaskSlotToRemove); break;

	case 4: AvailableSingleWorkerTaskSlots_4.Remove(SingleTaskSlotToRemove); break;

	case 5: AvailableSingleWorkerTaskSlots_5.Remove(SingleTaskSlotToRemove); break;

	default: AvailableSingleWorkerTaskSlots_6.Remove(SingleTaskSlotToRemove); break;
	}

	//AvailableSingleTaskSlots.Remove(SingleTaskSlotToRemove);
}

void UTaskManagerComponent::AddToAvailableSoldiers(AAI_Alive_Soldier* SoldierToAdd)
{
	if (!IsValid(SoldierToAdd))return;
	AvailableSoldiers.AddUnique(SoldierToAdd);
	AssignMilitaryTask();
}

void UTaskManagerComponent::RemoveFromAvailableSoldiers(AAI_Alive_Soldier* SoldierToRemove)
{
	if(AvailableSoldiers.Contains(SoldierToRemove))AvailableSoldiers.Remove(SoldierToRemove);
}

void UTaskManagerComponent::AddToAvailableWorkers(AAI_Alive_Worker* WorkerToAdd)
{
	if (!IsValid(WorkerToAdd))return;
	AvailableWorkers.AddUnique(WorkerToAdd);
	AssignWorkerTask();
}

void UTaskManagerComponent::RemoveFromAvailableWorkers(AAI_Alive_Worker* WorkerToRemove)
{
	if (AvailableWorkers.Contains(WorkerToRemove))AvailableWorkers.Remove(WorkerToRemove);
}

void UTaskManagerComponent::AssignMilitaryTask()
{
	if (AvailableSoldiers.Num() == 0)return;

	auto FirstAvailableTask = GetFirstAvailableSingleTasksByPriority();

	if (!IsValid(FirstAvailableTask)) return;
	AAI_Alive_Soldier* FirstAvailableSoldier = NULL;

	// --- First nonNull soldier ---

	for (int32 i = AvailableSoldiers.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(AvailableSoldiers[i]))AvailableSoldiers.RemoveAt(i);
		else if(IsValid(AvailableSoldiers[i]->CurrentSingleTask))AvailableSoldiers.RemoveAt(i);
		else{ FirstAvailableSoldier = AvailableSoldiers[i]; break; }
	}

	if (!IsValid(FirstAvailableSoldier))return;

	// --- Assigning ---
	//UE_LOG(LogTemp, Warning, TEXT("ASSIGNING TASK"))
	RemoveFromAvailableSingleTaskSlots(FirstAvailableTask);
	RemoveFromAvailableSoldiers(FirstAvailableSoldier);

	FirstAvailableSoldier->CurrentSingleTask = FirstAvailableTask;
	//FirstAvailableSoldier->isPerformingTask = true;
	FirstAvailableSoldier->ChangeBehavior(EBehaviorMode::GoingToTask);
	FirstAvailableTask->AssignedSoldier = FirstAvailableSoldier;
}

void UTaskManagerComponent::AssignWorkerTask()
{
	if (AvailableWorkers.Num() == 0)return;

	auto FirstAvailableTask = GetFirstAvailableSingleWorkerTasksByPriority();

	if (!IsValid(FirstAvailableTask)) return;
	AAI_Alive_Worker* FirstAvailableWorker = NULL;

	// --- First nonNull soldier ---

	for (int32 i = AvailableWorkers.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(AvailableWorkers[i]))AvailableWorkers.RemoveAt(i);
		else if (IsValid(AvailableWorkers[i]->CurrentSingleTask))AvailableWorkers.RemoveAt(i);
		else { FirstAvailableWorker = AvailableWorkers[i]; break; }
	}

	if (!IsValid(FirstAvailableWorker))return;

	// --- Assigning ---
	//UE_LOG(LogTemp, Warning, TEXT("ASSIGNING TASK"))
	RemoveFromAvailableSingleWorkerTaskSlots(FirstAvailableTask);
	RemoveFromAvailableWorkers(FirstAvailableWorker);

	FirstAvailableWorker->CurrentSingleTask = FirstAvailableTask;
	//FirstAvailableSoldier->isPerformingTask = true;
	FirstAvailableWorker->ChangeBehavior(EBehaviorMode::GoingToTask);
	FirstAvailableTask->AssignedWorker = FirstAvailableWorker;
}

ASingleTaskSlot* UTaskManagerComponent::GetFirstAvailableSingleTasksByPriority()
{
	if (AvailableSingleTaskSlots_0.Num() > 0)
	{
		for (int32 i = AvailableSingleTaskSlots_0.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleTaskSlots_0[i]))AvailableSingleTaskSlots_0.RemoveAt(i);

			return AvailableSingleTaskSlots_0[i];
		}
	}

	if (AvailableSingleTaskSlots_1.Num() > 0)
	{
		for (int32 i = AvailableSingleTaskSlots_1.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleTaskSlots_1[i]))AvailableSingleTaskSlots_1.RemoveAt(i);

			return AvailableSingleTaskSlots_1[i];
		}
	}

	if (AvailableSingleTaskSlots_2.Num() > 0)
	{
		for (int32 i = AvailableSingleTaskSlots_2.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleTaskSlots_2[i]))AvailableSingleTaskSlots_2.RemoveAt(i);

			return AvailableSingleTaskSlots_2[i];
		}
	}

	if (AvailableSingleTaskSlots_3.Num() > 0)
	{
		for (int32 i = AvailableSingleTaskSlots_3.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleTaskSlots_3[i]))AvailableSingleTaskSlots_3.RemoveAt(i);

			return AvailableSingleTaskSlots_3[i];
		}
	}

	if (AvailableSingleTaskSlots_4.Num() > 0)
	{
		for (int32 i = AvailableSingleTaskSlots_4.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleTaskSlots_4[i]))AvailableSingleTaskSlots_4.RemoveAt(i);

			return AvailableSingleTaskSlots_4[i];
		}
	}

	if (AvailableSingleTaskSlots_5.Num() > 0)
	{
		for (int32 i = AvailableSingleTaskSlots_5.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleTaskSlots_5[i]))AvailableSingleTaskSlots_5.RemoveAt(i);

			return AvailableSingleTaskSlots_5[i];
		}
	}

	if (AvailableSingleTaskSlots_6.Num() > 0)
	{
		for (int32 i = AvailableSingleTaskSlots_6.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleTaskSlots_6[i]))AvailableSingleTaskSlots_6.RemoveAt(i);

			return AvailableSingleTaskSlots_6[i];
		}
	}

		return NULL;
}

ASingleTaskSlot* UTaskManagerComponent::GetFirstAvailableSingleWorkerTasksByPriority()
{
	if (AvailableSingleWorkerTaskSlots_0.Num() > 0)
	{
		for (int32 i = AvailableSingleWorkerTaskSlots_0.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleWorkerTaskSlots_0[i]))AvailableSingleWorkerTaskSlots_0.RemoveAt(i);

			return AvailableSingleWorkerTaskSlots_0[i];
		}
	}

	if (AvailableSingleWorkerTaskSlots_1.Num() > 0)
	{
		for (int32 i = AvailableSingleWorkerTaskSlots_1.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleWorkerTaskSlots_1[i]))AvailableSingleWorkerTaskSlots_1.RemoveAt(i);

			return AvailableSingleWorkerTaskSlots_1[i];
		}
	}

	if (AvailableSingleWorkerTaskSlots_2.Num() > 0)
	{
		for (int32 i = AvailableSingleWorkerTaskSlots_2.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleWorkerTaskSlots_2[i]))AvailableSingleWorkerTaskSlots_2.RemoveAt(i);
			else
			return AvailableSingleWorkerTaskSlots_2[i];
		}
	}

	if (AvailableSingleWorkerTaskSlots_3.Num() > 0)
	{
		for (int32 i = AvailableSingleWorkerTaskSlots_3.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleWorkerTaskSlots_3[i]))AvailableSingleWorkerTaskSlots_3.RemoveAt(i);

			return AvailableSingleWorkerTaskSlots_3[i];
		}
	}

	if (AvailableSingleWorkerTaskSlots_4.Num() > 0)
	{
		for (int32 i = AvailableSingleWorkerTaskSlots_4.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleWorkerTaskSlots_4[i]))AvailableSingleWorkerTaskSlots_4.RemoveAt(i);

			return AvailableSingleWorkerTaskSlots_4[i];
		}
	}

	if (AvailableSingleWorkerTaskSlots_5.Num() > 0)
	{
		for (int32 i = AvailableSingleWorkerTaskSlots_5.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleWorkerTaskSlots_5[i]))AvailableSingleWorkerTaskSlots_5.RemoveAt(i);

			return AvailableSingleWorkerTaskSlots_5[i];
		}
	}

	if (AvailableSingleWorkerTaskSlots_6.Num() > 0)
	{
		for (int32 i = AvailableSingleWorkerTaskSlots_6.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(AvailableSingleWorkerTaskSlots_6[i]))AvailableSingleWorkerTaskSlots_6.RemoveAt(i);

			return AvailableSingleWorkerTaskSlots_6[i];
		}
	}

	return NULL;
}