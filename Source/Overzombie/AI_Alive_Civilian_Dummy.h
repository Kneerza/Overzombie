// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Alive_Civilian.h"
#include "AI_Alive_Civilian_Dummy.generated.h"

class ADistructibleStructure;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AAI_Alive_Civilian_Dummy : public AAI_Alive_Civilian
{
	GENERATED_BODY()
	
public:

	AAI_Alive_Civilian_Dummy();

	virtual void ActivateAI() override;

	virtual void ChangeBehavior(EBehaviorMode NewBehaviorMode) override;

	virtual void RunFromZombies() override;

	UPROPERTY()
		ADistructibleStructure* ParentAsDisStructure;

	UFUNCTION()
		void DamageParentDisStructure(float Damage);
};
