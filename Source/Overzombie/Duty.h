// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Duty.generated.h"

class ATargetPoint;
class AAI_Duty;
class UFloatingPawnMovement;
class UTaskManagerComponent;

UCLASS()
class OVERZOMBIE_API ADuty : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADuty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UFloatingPawnMovement* MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UTaskManagerComponent* TaskManagerComponent;

	UPROPERTY()
		float DutySpeed = 400.f;

	UPROPERTY(BlueprintReadOnly)
		AAI_Duty* ControllingAI;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --- Components ---

	UPROPERTY(EditAnywhere)
		TArray<ATargetPoint*> DutySlots;
	// ------

	UFUNCTION(BlueprintCallable, Category = Custom)
		void InitializeSlots();

	UFUNCTION(BlueprintCallable, Category = Custom)
		void InitializeAI();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
		TArray<ATargetPoint*> GetDutySlots();
	UFUNCTION(BlueprintPure)
		TArray<ATargetPoint*> GetPatrollingPoints();

	UPROPERTY(EditAnywhere, Category = Setup)
		float OverwatchingTime = 4.f;


private:

	UPROPERTY(EditAnywhere, Category = Setup)
		TArray<ATargetPoint*> PatrollingPoints;	
};
