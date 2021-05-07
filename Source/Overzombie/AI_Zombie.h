// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Overzombie.h"
#include "AI_Zombie.generated.h"

class APawn_Zombie;
class APawn_Alive;
class APsychoSphere;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AAI_Zombie : public AAI_Overzombie
{
	GENERATED_BODY()
	
		virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		float Elevation = 130.f;

	UPROPERTY()
	bool IsActive = false;

	UPROPERTY()
		bool isUnderControl = false;

	UFUNCTION()
	void ActivateAI();

	UFUNCTION()
	void Bite();

	UPROPERTY(BlueprintReadWrite, Category = OZ_Zombie)
	FVector PointOfInterest;

	UPROPERTY()
		APawn_Zombie* PossessedZombie_Ref;
	UPROPERTY()
		float ZombiePivotOffset = 10.f;

	virtual void ChangeBehavior(EBehaviorMode NewBehaviorMode) override;

	UFUNCTION()
	void SetPointOfInterest();

	void FloatInRandomDirection();
	void ChargeForward();

	FVector DirBeforeFalling;

	float CurrentChargingTime = 15;
	float ChargingTime = 15;
	float StartChargingTime = 0;

	UFUNCTION(BlueprintCallable, Category = OZ_Zombie)
		void FloatToLocation(FVector LocationFloatTo, float Speed = 100.f);

	UPROPERTY()
	float TimeBeforeSetNewPointOfInterest = 1;

	UPROPERTY()
	APawn_Alive* AliveToConsume;

	UPROPERTY()
	TArray<APawn_Alive*> AllAlives_Hearing;

	UPROPERTY()
	float BitingDistance = 200;

	UPROPERTY()
	float Damage = 30;

	UPROPERTY()
	float RechargingTime = 0.4;

	UPROPERTY()
	float TimeOfLastBite = 0;

	UPROPERTY()
	FRotator PlayerRot;

	UPROPERTY()
	FVector OffsetFromCurrentTarget;

	UFUNCTION()
	void GetClosestAlive();

	UFUNCTION()
	void ChangeAliveToConsume();

	UPROPERTY()
	float NotFollowDistance = 2000;

	UPROPERTY()
		APsychoSphere* PsychoSphere;
	UPROPERTY()
		FVector LastPsychoSphereLoc;

	UPROPERTY()
		FVector PursuingDummyOffset;

	UPROPERTY(BlueprintReadWrite)
		float SpeedBuff;

	UFUNCTION(BlueprintCallable)
		void ApplySpeedBuff(float SpeedBuffAmount);
	UFUNCTION(BlueprintCallable)
		void RemoveSpeedBuff();

	UFUNCTION(BlueprintCallable)
		void SetIsFalling(bool NewIsFalling);
	UFUNCTION(BlueprintPure)
		bool GetIsFalling();

	UPROPERTY()
		bool isRaising = false;

	void TurnToLocation(FVector LocationTurnTo);

private:

	UPROPERTY()
	bool isFalling = false;

};
