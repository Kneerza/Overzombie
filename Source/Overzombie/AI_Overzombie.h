// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_Overzombie.generated.h"

class UTaskManagerComponent;

UENUM(BlueprintType)
enum class EBehaviorMode : uint8
{
	Idle,
	Pursuing,
	GoingToTask,
	WorkingOnTask,
	WorkingOnInfiniteTask,
	Shooting,
	GoingOnDuty,
	OnDuty,
	Waiting,
	ChargeForward,
	Purpose
};

class AOverzombieController;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AAI_Overzombie : public AAIController
{
	GENERATED_BODY()
	

		

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AOverzombieController* OverzombieController_Ref;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float FloatingSpeed = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float WalkingSpeed = 450;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float RunningSpeed = 600;

	//--------------------Behavior-------------------------------
	UPROPERTY(EditAnywhere, Category = Setup)
		EBehaviorMode CurrentBehaviorMode = EBehaviorMode::Idle;

	UFUNCTION(BlueprintCallable)
	virtual void ChangeBehavior(EBehaviorMode NewBehaviorMode);

	//-----------------------------------------------------------

	virtual void BeginPlay() override;

	// -------------GLOBAL Variiables--------------------------

	float G_PointOfInterestRadius = 4000.f;

	float G_HuddleOffsetFromPointOfInterest = 800.f;

	UPROPERTY()
		UTaskManagerComponent* TaskManagerComponent;

protected:

	UPROPERTY()
		float EndWaitingTime = 0.f;
	UPROPERTY()
		float WaitingPeriodSeconds = 1.f;

};
