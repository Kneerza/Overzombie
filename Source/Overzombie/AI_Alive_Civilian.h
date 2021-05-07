// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Overzombie_Alive.h"
#include "AI_Alive_Civilian.generated.h"


class APawn_Alive_Civilian;
class APawn_Zombie;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AAI_Alive_Civilian : public AAI_Overzombie_Alive
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;

		APawn_Alive_Civilian* PossessedAliveCivilian_Ref;

		bool IsActive = false;

		virtual void ActivateAI();

		UPROPERTY(BlueprintReadWrite)
			FVector PointOfInterest;

		UFUNCTION(BlueprintCallable, Category = OZ_Civillian)
			void RoamAround();

		UPROPERTY()
			float RunAwayDistanceModifier = 1;

		void SetPointOfInterest();

		UPROPERTY()
		float TimeBeforeSetNewPointOfInterest = 1;
		UPROPERTY()
			float RoamAround_MinWaitingTimeBeforeChangeingDirection = 5;
		UPROPERTY()
			float RoamAround_MaxWaitingTimeBeforeChangeingDirection = 8;
		UPROPERTY()
			float PointOfInterest_MinDistance = 1000;
		UPROPERTY()
			float PointOfInterest_MaxDistance = 3000;

		//UPROPERTY()
		//	float Elevation = 130.f;

		//TArray<APawn_Zombie*> AllZombies_Hearing;

		FVector GetRunFromZombiesDirection();

		virtual void ChangeBehavior(EBehaviorMode NewBehaviorMode) override;

		UFUNCTION()
			virtual void RunFromZombies();

		float AttnRadius = 140.f;

};
