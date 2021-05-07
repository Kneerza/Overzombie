// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawnery.generated.h"

class AOverzombieController;
class APawn_Overzombie;

UCLASS()
class OVERZOMBIE_API ASpawnery : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnery();

	// --- Classes ---

	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn_Overzombie> PawnClassToSpawn;
	// ------


	UPROPERTY()
		AOverzombieController* PlayerController_Ref;

	UPROPERTY()
		int32 WaitingToSpawnCounts = 0;
	

	UFUNCTION()
		virtual void Spawn();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RechargeTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float OriginalRechargeTime = 2.f;

	UPROPERTY()
		float LastSpawnTime = 0.f;

	UPROPERTY()
		bool  isMilitary = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:



};
