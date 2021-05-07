// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawnery.h"
#include "ZombieSpawnery.generated.h"

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AZombieSpawnery : public ASpawnery
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RallyPointOffset = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isInactive = false;
	
	virtual void Spawn() override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Setup)
		bool isOneCharge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool isModifyingRechargeTimeAfterEachSpawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool areZombiesCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ChargingTime = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RandAngleRange = 20.f;
};
