// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Alive.h"
#include "Pawn_Alive_Worker.generated.h"

class APawn_Zombie;
class AAI_Overzombie;
class AAI_Alive_Worker;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API APawn_Alive_Worker : public APawn_Alive
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;



public:

	virtual void Kill() override;

	UPROPERTY(BlueprintReadOnly, Category = OZ_Civillian)
		AAI_Alive_Worker* AI_Alive_Worker_Ref;

	UPROPERTY(EditAnywhere, Category = OZ_Setup)
		float RunAwayDistanceModifier = 1;

	UPROPERTY(EditAnywhere, Category = Setup)
		TSubclassOf<AAI_Overzombie> AI_Possessing_Class;

	UPROPERTY(EditAnywhere, Category = OZ_Setup)
		float PointOfInterest_MinDistance = 1000;
	UPROPERTY(EditAnywhere, Category = OZ_Setup)
		float PointOfInterest_MaxDistance = 3000;

	virtual void BeginHearing(AActor* ActorInRange) override;

	virtual void EndHearing(AActor* ActorOutOfRange) override;

	virtual void InitializeAI() override;
};
