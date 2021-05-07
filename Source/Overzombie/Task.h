// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Task.generated.h"

class AAI_Alive_Soldier;
class UTaskManagerComponent;
class USlotComponent;
class USceneComponent;
class USphereComponent;
class UBoxComponent;

UCLASS()
class OVERZOMBIE_API ATask : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATask();

	UPROPERTY()
		int32 ZombiesCount = 0;

	UFUNCTION(BlueprintCallable)
		void Initialize_BP();

	UFUNCTION()
		void OnZombieEnterDestructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnZombieLeaveDestructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnWorkerEnterConstructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnWorkerLeaveConstructionArea(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
		TArray<USlotComponent*> AllSlots;
	UPROPERTY()
		TArray<USlotComponent*> AvailableSlots;

	UFUNCTION()
		void InitializeSlots();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnComplete"))
		void OnComplete();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDestructed"))
		void OnDestructed();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDamaged"))
		void OnDamaged();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnZombieEntered"))
		void OnZombieEntered();

	UPROPERTY()
		TArray<AAI_Alive_Soldier*> CurrentlyAssignedSoldiers;

	UFUNCTION()
		USlotComponent* AssignSoldier(AAI_Alive_Soldier* SoldierToAssign);
	UFUNCTION()
		void DismissSoldier(AAI_Alive_Soldier* SoldierToDismiss);

	UFUNCTION()
		int32 GetCapacity();

	UFUNCTION()
		USlotComponent* GetAvailableSlot();

	// --- Outer Objects ---

	UPROPERTY()
		UTaskManagerComponent* TaskManagerComponent;
	// ------

	UPROPERTY()
		bool isAvailable = true;

	UPROPERTY()
		bool isComplete = true;

	UFUNCTION()
		bool AddTaskProgress(float AmountToAdd);

	UFUNCTION()
		void InitializeTask(UTaskManagerComponent* TaskManagerComponentRef);

	UPROPERTY(EditAnywhere)
		int32 Priority = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		bool isEndGameTask = false;

	UPROPERTY(EditAnywhere, Category = Setup)
		int32 Capacity = 3;
	UPROPERTY(EditAnywhere, Category = Setup)
		int32 TaskDifficulty = 2000;
	UPROPERTY()
		float TaskProgress = 0.f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* DestructionArea;
	UPROPERTY(EditAnywhere)
		UBoxComponent* ConstructionArea;

	UPROPERTY(EditAnywhere, Category = Setup)
		int32 HP = 1200;
	UPROPERTY()
		int32 CurrentHP = 1200;

	UPROPERTY()
		int32 CurrentWorkers = 0;

	UPROPERTY(EditAnywhere, Category = Setup)
		bool isKillingEnteringZombies = false;
	UPROPERTY(EditAnywhere, Category = Setup)
		bool isGettingDamageByZombies = true;

	UPROPERTY(BlueprintReadOnly)
		int32 ZombiesEntered = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 ZombiesToEnterForDestruction = 40;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




};

