// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Overzombie.h"
#include "Pawn_Alive.generated.h"

class USphereComponent;
class AAI_Overzombie;
class APawn_Zombie;
class AAI_Overzombie_Alive;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API APawn_Alive : public APawn_Overzombie
{
	GENERATED_BODY()
	
public:

	APawn_Alive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		USphereComponent* Hearing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		USphereComponent* NoiseNSmell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float HearingRadius = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float NoiseNSmellRadius = 2000;

	UPROPERTY()
		bool isDying = false;

	UPROPERTY(EditAnywhere, Category = Setup)
		bool isDummy = false;
	
	UFUNCTION()
		virtual bool RecieveDamage(float DamageToRecieve);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	AAI_Overzombie* AI_Overzombie;

	AAI_Overzombie_Alive* AI_Alive_Ref;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		virtual void BeginHearing(AActor* ActorInRange);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		virtual void EndHearing(AActor* ActorOutOfRange);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		virtual void OnBeginNoiseNSmell(AActor* ActorInRange);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		virtual void OnEndNoiseNSmell(AActor* ActorOutOfRange);

	virtual void InitializeAI();

	TArray<APawn_Zombie*> AllZombies_Hearing;
};
