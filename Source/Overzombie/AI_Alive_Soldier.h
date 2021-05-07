// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Overzombie_Alive.h"
#include "AI_Alive_Soldier.generated.h"

class APawn_Alive_Soldier;
class APawn_Zombie;
class ADuty;
class ATask;
class USlotComponent;
class ASingleTaskSlot;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AAI_Alive_Soldier : public AAI_Overzombie_Alive
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		void ReleaseTask();

	UPROPERTY(EditAnywhere, Category = Tracking)
		ATask* CurrentTask;
	UPROPERTY(EditAnywhere, Category = Tracking)
		USlotComponent* CurrentTaskSlot;
	UPROPERTY(EditAnywhere, Category = Tracking)
		APawn_Alive_Soldier* PossessedAliveSoldier_Ref;

	UFUNCTION()
		void AssignTask();

	TArray<APawn_Zombie*> AllZombies_Vision;

	virtual void Tick(float DeltaTime) override;

	bool IsActive = false;
	APawn_Zombie* CurrentShootingTarget = nullptr;

	void ActivateAI();

	UPROPERTY(EditAnywhere, Category = Setup)
		float WorkEfficiency = 1.f;

	UPROPERTY(EditAnywhere, Category = Setup)
		float WorkProgressRateSeconds = 1.f;

	UPROPERTY()
		float LastWorkProgressTime = 0.f;

	virtual void ChangeBehavior(EBehaviorMode NewBehaviorMode) override;
	


	void TurnToActor(AActor* ActorToTurnTo);

	UFUNCTION()
		void EnlistOnAvailableDuty();

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		UStaticMeshComponent* FireSocket = NULL;		// Initialize in BP

	// --- Access to variables ---

	UFUNCTION(BlueprintPure)
		ATask* GetCurrentTask();
	UFUNCTION()
		void SetCurrentTask(ATask* NewCurrentTask);

	UFUNCTION(BlueprintPure)
		USlotComponent* GetCurrentTaskSlot();
	UFUNCTION()
		void SetCurrentTaskSlot(USlotComponent* NewCurrentTaskSlot);

	UFUNCTION()
		void SetCurrentDuty(ADuty* NewCurrentDuty);
	UFUNCTION(BlueprintPure)
		ADuty* GetCurrentDuty();
	UFUNCTION()
		void SetCurrentDutySlot(ATargetPoint* NewCurrentDutySlot);

	UFUNCTION(BlueprintPure)
		bool GetisOnDuty();
	UFUNCTION()
		void SetisOnDuty(bool NewisOnDuty);

	UFUNCTION()
		void SetupWeapon(float NewReloadingSpeed, float NewDamage, bool NewIsSquareDamage, float NewWeaponNoiseRadius, float NewAimingTime);

	UFUNCTION()
		void SetIsAvailableForDuty(bool NewIsAvailableForDuty);
	UFUNCTION()
		bool GetIsAvailableForDuty();

	UFUNCTION()
		void SetReactionDistance(float NewReactionDistance);
	UFUNCTION(BlueprintPure)
		float GetReactionDistance();
	// ------

	UPROPERTY()
		ASingleTaskSlot* CurrentSingleTask;

	//UPROPERTY()
	//	bool isPerformingTask = false;


private:



	UPROPERTY()
		bool isOnDuty = false;
	UPROPERTY()
		ADuty* CurrentDuty = NULL;
	UPROPERTY()
		ATargetPoint* CurrentDutySlot = NULL;

	UFUNCTION()
		void TurnToLocation(FVector LocationTurnTo);

	UPROPERTY()
		float ReloadingSpeed = 0.6;
	UPROPERTY()
		float AimingTime = 0.2;
	UPROPERTY()
		float StartAimingTimeSeconds = 0.f;
	UPROPERTY()
		float LastShotTime = 0;
	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		float Damage = 60;
	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		float WeaponNoiseRadius = 1200;
	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		bool isSquareDamage = false;

	///UFUNCTION()
		///TArray<APawn_Zombie*> GetAllZombies_VisionCone();

	void Shoot();

	APawn_Zombie* GetClosestZombie_Hearing();

	float VisionAngle = 60;

	UPROPERTY()
		float ReactionDistance = 600;

	UFUNCTION()
		void TurnTowardsNoise(AActor* ActorTurnTo);

protected:

	UPROPERTY()
		float isAvailableForDuty = true;
};
