// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Alive.h"
#include "OverzombieController.h"
#include "Pawn_Alive_Soldier.generated.h"

class USphereComponent;
class AAI_Alive_Soldier;
class APawn_Zombie;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API APawn_Alive_Soldier : public APawn_Alive
{
	GENERATED_BODY()

		APawn_Alive_Soldier();
public:

	virtual void BeginPlay() override;

	virtual void Kill() override;

	virtual void OnBeginNoiseNSmell(AActor* ActorInRange) override;

	virtual void OnEndNoiseNSmell(AActor* ActorOutOfRange) override;

	//UFUNCTION(BlueprintCallable, Category = Weapon)
	//	void SetFireSocket(UStaticMeshComponent* SocketLocation);

	///UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	///	USphereComponent* Vision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UStaticMeshComponent* FireSocket = NULL;

	///UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	///	float VisionRadius = 1600;

	UPROPERTY()
	AAI_Alive_Soldier* AI_Alive_Soldier_Ref;
	

	UPROPERTY(EditAnywhere, Category = Setup)
		TSubclassOf<AAI_Overzombie> AI_Possessing_Class;

	virtual void BeginHearing(AActor* ActorInRange) override;

	virtual void EndHearing(AActor* ActorOutOfRange) override;

	/*
	UFUNCTION(BlueprintCallable, Category = "Custom")
		virtual void BeginVision(AActor* ActorInRange);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		virtual void EndVision(AActor* ActorOutOfRange);
*/
	virtual void InitializeAI() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool isInVehicle = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AActor* VehicleSlotLocation = NULL;

	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		EShootEffectType ShootEffectType;

protected:

	virtual void Initialize_BP() override;

	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		float ReloadingSpeed = 0.6;
	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		float Damage = 60;
	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		float WeaponNoiseRadius = 1200;
	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		float AimingTime = 0.2;
	UPROPERTY(EditAnywhere, Category = Setup_Weapon)
		bool isSquareDamage = false;

	UPROPERTY(EditAnywhere, Category = Setup)
		bool isAvailableForDuty = true;

	UPROPERTY(EditAnywhere, Category = Setup)
		float ReactionDistance = 600;
};
