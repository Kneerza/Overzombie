// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DistructibleStructure.h"
#include "DistructibleStructure_MultiStage.generated.h"

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API ADistructibleStructure_MultiStage : public ADistructibleStructure
{
	GENERATED_BODY()
	
public:

	virtual void OnHit() override;

	virtual void RecieveDamage(float DamageToRecieve) override;

protected:

	UFUNCTION()
		void Damage();

	UPROPERTY()
		bool isDamaged = false;
};
