// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverzombieController.generated.h"

UENUM(BlueprintType)
enum class EShootEffectType : uint8
{
	Pistol,
	AR
};

class UTaskManagerComponent;
class ATargetPoint;
class APawn_Alive;
class APawn_Zombie;
class UParticleSystemComponent;
class ASpawnery;
class AAI_Alive_Soldier;
class AAI_Alive_Worker;
class AAI_Zombie;
class APsychoSphere;
class UOZWidget;
class AAI_Alive_Civilian;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AOverzombieController : public APlayerController
{
	GENERATED_BODY()
	
		virtual void BeginPlay() override;

public:

	AOverzombieController();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//	ATargetPoint* OverzombieTarget;

	

	UFUNCTION(BlueprintCallable)
		void GameOver(bool isWin);


	// --- Components ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTaskManagerComponent* TaskManagerComponent;
	// ------


	// --- Outer Objects ---

	UPROPERTY()
		APsychoSphere* PsychoSphere;
	// ------


	// --- Tracking ---

	UPROPERTY()
		TArray<ASpawnery*> Spawnaries;
	UPROPERTY(BlueprintReadOnly)
		TArray<AAI_Alive_Soldier*> AllSoldiers;
	UPROPERTY(BlueprintReadOnly)
		TArray<AAI_Zombie*> NEW_AllZombies;
	UPROPERTY(BlueprintReadOnly)
		TArray<AAI_Alive_Worker*> AllWorkers;
	UPROPERTY(BlueprintReadOnly)
		TArray<AAI_Alive_Civilian*> AllCivilians;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 CurrentMaxSoldiersCount = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 CurrentMaxWorkersCount = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 CurrentMaxZombiesCount = 100;

	// ------


	//-------- Class to spawn --------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<APawn_Zombie> ZombieBPClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		class UParticleSystem* EffectSoldierPistolShootClassToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		class UParticleSystem* EffectSoldierARShootClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		class UParticleSystem* EffectBleedClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<APsychoSphere> PsychoSphereClassToSpawn;

	UPROPERTY(EditAnywhere, Category = Setup)
		TSubclassOf<UOZWidget> EndGameScreenClassToSpawn;
	//--------------------------------------


	// --- Spawneries Call ---

	UFUNCTION()
		void SpawnSoldiers();
	// ------


	UPROPERTY(EditAnywhere)
		float MinSphereRadius = 1000;
	UPROPERTY(EditAnywhere)
		float MaxSphereRadius = 3000;


	// --- Statistics ---

	UPROPERTY(BlueprintReadOnly, Category = OZ_Statistics)
		int32 AllZombies = 0;
	// ------


	// --- Events in BP called in C++ ---

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CheckSpawneries"))
		void CheckSpawneries();
	// ------

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		float CurrentBaitingDistance = 6000.f;



	void TurnAliveIntoZombie(APawn_Alive* AliveToTurn);

	UPROPERTY(BlueprintReadOnly, Category = OZ)
	ATargetPoint* CurrentTarget;

	UPROPERTY()
		FVector CurrentCursorLocation;

	UFUNCTION()
		void FindAllFloorElevations();
	UFUNCTION()
		float GetClosestFloorElevation(float Elevation);
	UFUNCTION()
		bool GetNextLowerFloorElevation(float Elevation, float &OUT_LowerElevation);

	float G_PointOfInterestRadius = 4000.f;

	float G_HuddleOffsetFromPointOfInterest = 800.f;

	float G_HuddleOffsetFromPointOfInterestMIN = 0.f;

	float G_HuddleOffsetFromPointOfInterestMAX = 0.f;

	virtual void Tick(float DeltaTime) override;


	// ------------ Spawn Effects ---------------

	void Effect_Shoot(EShootEffectType ShootEffectType, FTransform Transform);
	void Effect_Bleed(FVector Location);


	// --- Input Events and Handlers ---

	static FName LargeAreaInteraction;
	static FName SmallAreaInteraction;

	static FName Zoom;
	static FName MoveLeftRight;
	static FName MoveUpDown;

	void LargeAreaInteraction_Press_Handle();
	void LargeAreaInteraction_Release_Handle();
	void SmallAreaInteraction_Press_Handle();
	void SmallAreaInteraction_Release_Handle();
	// ------

protected:

		virtual void SetupInputComponent() override;

		UPROPERTY()
			bool isLBPressed = false;
		UPROPERTY()
			bool isRBPressed = false;

		UPROPERTY()
			TArray<float> FloorElevations;
};
