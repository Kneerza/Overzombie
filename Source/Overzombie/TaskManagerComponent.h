// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TaskManagerComponent.generated.h"

class ATask;
class ADuty;
class AAI_Alive_Soldier;
class ASingleTaskSlot;
class AAI_Alive_Worker;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERZOMBIE_API UTaskManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTaskManagerComponent();


	UPROPERTY()
		TArray<ATask*> AvailableTasks_0;
	UPROPERTY()
		TArray<ATask*> AvailableTasks_1;
	UPROPERTY()
		TArray<ATask*> AvailableTasks_2;
	UPROPERTY()
		TArray<ATask*> AvailableTasks_3;
	UPROPERTY()
		TArray<ATask*> AvailableTasks_4;
	UPROPERTY()
		TArray<ATask*> AvailableTasks_5;
	UPROPERTY()
		TArray<ATask*> AvailableTasks_6;

	UPROPERTY()
		TArray<ATask*> AllTasks_0;
	UPROPERTY()
		TArray<ATask*> AllTasks_1;
	UPROPERTY()
		TArray<ATask*> AllTasks_2;
	UPROPERTY()
		TArray<ATask*> AllTasks_3;
	UPROPERTY()
		TArray<ATask*> AllTasks_4;
	UPROPERTY()
		TArray<ATask*> AllTasks_5;
	UPROPERTY()
		TArray<ATask*> AllTasks_6;

	UPROPERTY()
		TArray<ADuty*> AvailableDuties;

	UFUNCTION()
		void AssignSoldier(AAI_Alive_Soldier* SoldierToAssign);

	UFUNCTION()
		void MakeTaskUnavailable(ATask* TaskToMakeUnavailable);
	UFUNCTION()
		void MakeTaskAvailable(ATask* TaskToMakeAvailable, int32 Priority);

	UFUNCTION()
		void PullHigherPriorityTaskSoldiersToLowerPriority(int32 PriorityPullTo, int32 NumberOfSoldiersToPull);

	UFUNCTION()
		ATask* GetFirstPriorityTask();

	UPROPERTY()
		int32 GameOverCount =0;

	UFUNCTION()
		void CompleteGameOverTask();

	UFUNCTION()
		void AddToAvailableSingleTaskSlots(ASingleTaskSlot* SingleTaskSlotToAdd);
	UFUNCTION()
		void RemoveFromAvailableSingleTaskSlots(ASingleTaskSlot* SingleTaskSlotToRemove);

	UFUNCTION()
		void AddToAvailableSingleWorkerTaskSlots(ASingleTaskSlot* SingleTaskSlotToAdd);
	UFUNCTION()
		void RemoveFromAvailableSingleWorkerTaskSlots(ASingleTaskSlot* SingleTaskSlotToRemove);

	UFUNCTION()
		void AddToAvailableSoldiers(AAI_Alive_Soldier* SoldierToAdd);
	UFUNCTION()
		void RemoveFromAvailableSoldiers(AAI_Alive_Soldier* SoldierToRemove);

	UFUNCTION()
		void AddToAvailableWorkers(AAI_Alive_Worker* WorkerToAdd);
	UFUNCTION()
		void RemoveFromAvailableWorkers(AAI_Alive_Worker* WorkerToRemove);

	UFUNCTION()
		void AssignMilitaryTask();

	UFUNCTION()
		void AssignWorkerTask();

	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots;

	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots_0;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots_1;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots_2;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots_3;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots_4;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots_5;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleTaskSlots_6;

	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleWorkerTaskSlots_0;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleWorkerTaskSlots_1;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleWorkerTaskSlots_2;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleWorkerTaskSlots_3;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleWorkerTaskSlots_4;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleWorkerTaskSlots_5;
	UPROPERTY(BlueprintReadOnly)
		TArray<ASingleTaskSlot*> AvailableSingleWorkerTaskSlots_6;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	UPROPERTY()
		TArray<AAI_Alive_Soldier*> AvailableSoldiers;

	UPROPERTY()
		TArray<AAI_Alive_Worker*> AvailableWorkers;

	UFUNCTION()
		ASingleTaskSlot* GetFirstAvailableSingleTasksByPriority();

	UFUNCTION()
		ASingleTaskSlot* GetFirstAvailableSingleWorkerTasksByPriority();
		
};
