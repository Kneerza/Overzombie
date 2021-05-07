// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DistructibleStructure.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;
class ATask;

UCLASS()
class OVERZOMBIE_API ADistructibleStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADistructibleStructure();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDestructed"))
		void OnDestructed();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDamaged"))
		void OnDamaged();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTimerGoesOff"))
		void OnTimerGoesOff();


	// --- Components ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UStaticMeshComponent* Structure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UBoxComponent* CollisionBox;
	// ------


	// --- Events called from BP ---

	UFUNCTION(BlueprintCallable, Category = OZ_DistructibleStructure)
		virtual void OnHit();
	// ------

	UPROPERTY()
		ATask* ParentTask;

	UFUNCTION()
		virtual void RecieveDamage(float DamageToRecieve);

	UPROPERTY(EditAnywhere)
		bool isDisablingCollisionWhenDestructed = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// --- Setup Parameters ---

	UPROPERTY(EditAnywhere, Category = OZ_DistructibleStructure)
		float HP = 120;
	UPROPERTY(EditAnywhere, Category = OZ_DistructibleStructure)
		float CurrentHP = 120;
	UPROPERTY(EditAnywhere, Category = OZ_DistructibleStructure)
		int32 Constitution = 0;		//Shows how many zombies it can stand without taking any damage
	UPROPERTY()
		float FallingDirection = 1;		// 1 or -1
	UPROPERTY(EditAnywhere, Category = OZ_DistructibleStructure_DEV)
		FVector OffsetWhenDestructed = FVector(0.f,0.f,100.f);
	UPROPERTY(EditAnywhere, Category = OZ_DistructibleStructure)
		float FallingAmplitudeModifier = 0.1;		// How much Structure changes its rotation when hit
	UPROPERTY(EditAnywhere, Category = OZ_DistructibleStructure)
		float MaxAmplitudeAngles = 30;		// 1 or -1
	// ------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		FRotator DestroyedAngle = FRotator(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category = Setup)
		bool shouldBeDestroyedInBPOnly = false;
	UFUNCTION()
		void Destruct();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
		FRotator InitialRelativeRotation;



};
