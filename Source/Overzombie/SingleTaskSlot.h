// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SingleTaskSlot.generated.h"

class UTaskManagerComponent;
class UBoxComponent;
class AAI_Alive_Soldier;
class AAI_Alive_Worker;
class ATask;

UCLASS()
class OVERZOMBIE_API ASingleTaskSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingleTaskSlot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void Delete();

	UPROPERTY()
		bool isOccupied = false;

	UPROPERTY()
		AAI_Alive_Soldier* AssignedSoldier;
	UPROPERTY()
		AAI_Alive_Worker* AssignedWorker;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		bool AddTaskProgress(float AmountToAdd);

	UFUNCTION(BlueprintCallable)
		void Kill();

	UFUNCTION()
		void MakeAvailable();

	UFUNCTION()
		int32 GetPriority();
	UFUNCTION()
		void SetPriority(int32 NewPriority);

	UPROPERTY(EditAnywhere)
		int32 Priority = 0;

private:

	UPROPERTY()
		UTaskManagerComponent* TaskManagerComponent;
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxCollisionComponent;

	UPROPERTY()
		ATask* OwnerTask = NULL;

	UPROPERTY(EditAnywhere, Category = Setup)
		bool isTaskInfinite = false;

	UPROPERTY(EditAnywhere, Category = Setup)
		bool isMilitary = false;


};
