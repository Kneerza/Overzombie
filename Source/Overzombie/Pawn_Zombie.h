// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Overzombie.h"
#include "Pawn_Zombie.generated.h"

class AAI_Overzombie;
class AAI_Zombie;
class APsychoSphere;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API APawn_Zombie : public APawn_Overzombie
{
	GENERATED_BODY()
	
		virtual void BeginPlay() override;

public:

	APawn_Zombie();

	virtual void Kill() override;

	UPROPERTY(EditAnywhere, Category = Setup)
		TSubclassOf<AAI_Overzombie> AI_Possessing_Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
		AAI_Zombie* AI_Zombie_Ref;

	UPROPERTY()
		FVector RallyPoint = FVector(0);

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
		APsychoSphere* PsychoSphere;

	UFUNCTION(BlueprintCallable)
		void SetIsFalling(bool NewIsFalling);
	UFUNCTION(BlueprintPure)
		bool GetIsFalling();

	UPROPERTY()
		bool isFalling = false;

private:



	///UFUNCTION(BlueprintCallable, Category = OZ_Zombie)
	///	void SetPointOfInterest(FVector NewPointOfInterest);

	///UPROPERTY()
	///	FVector PointOfInterest = FVector(0);
};
