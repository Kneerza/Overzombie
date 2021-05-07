// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Overzombie.generated.h"

class AOverzombieController;
class UFloatingPawnMovement;
class UCapsuleComponent;
class USceneComponent;

UCLASS()
class OVERZOMBIE_API APawn_Overzombie : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawn_Overzombie();

	UFUNCTION(BlueprintCallable, Category = OZ)
		virtual void Kill();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float FloatingSpeedToOverride = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float WalkingSpeedToOverride = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float RunningSpeedToOverride = 0;


	// -------------------Components-----------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UFloatingPawnMovement* MovementComponent;
	//-----------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	AOverzombieController* OverzombieController_Ref;

	// --------------------STATS---------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	float HP = 120;

	//-----------------------------------------

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, Category = OZ_Initialization)
		virtual void Initialize_BP();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
