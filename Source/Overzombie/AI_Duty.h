// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_Duty.generated.h"

class ADuty;
class AAI_Alive_Soldier;
class ATargetPoint;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AAI_Duty : public AAIController
{
	GENERATED_BODY()
	
public:
	AAI_Duty();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetPossessedDuty(ADuty* NewPossessedDuty);
	UFUNCTION()
		void EnlistAvailableSoldiers();
	UFUNCTION()
		void InitializeSlots();
	UFUNCTION()
		void EnlistAvailableSoldier(AAI_Alive_Soldier* SoldierToEnlist);
	UFUNCTION()
		void RemoveEnlistedSoldier(AAI_Alive_Soldier* SoldierToRemove);

	UPROPERTY()
		bool isWaiting;

	//UPROPERTY()
		//float DutySpeed = 400.f;

	// --- Access to variables ---

	UFUNCTION()
		void SetHasAvailableSlots(bool NewhasAvailableSlots);
	UFUNCTION(BlueprintPure)
		bool GetHasAvailableSlots();

	UFUNCTION(BlueprintPure)
		int32 GetAvailableSlotsCount();

	UFUNCTION()
		void SetOverwatchingTime(float NewOverwatchingTime);
	// ------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ATargetPoint*> PatrollingPoints;

private:

	UPROPERTY()
		ADuty* PossessedDuty;

	UPROPERTY()
		bool hasAvailableSlots = true;
	UPROPERTY()
		float AvailableSlotsCount = 0;
	UPROPERTY()
		float SoldiersOnDutyCount = 0;
	UPROPERTY()
		TArray<ATargetPoint*> DutySlots;


	UPROPERTY()
		ATargetPoint* CurrentPatrollingPoint = NULL;

	UPROPERTY()
		TArray<AAI_Alive_Soldier*> EnlistedSoldiers;

	UFUNCTION(BlueprintCallable)
		void ChangeCurrentPatrollingPoint();

	UFUNCTION()
		void UpdateAvailableSlotsCount();

	UPROPERTY()
		bool isOverwatching = false;
	UPROPERTY()
		float StartOverwatchingTime = 0.f;
	UPROPERTY()
		float OverwatchingTime = 4.f;
	UPROPERTY()
		ATargetPoint* PatrollingPointWhenOverwatchOver = NULL;


protected:

	virtual void BeginPlay() override;
};
