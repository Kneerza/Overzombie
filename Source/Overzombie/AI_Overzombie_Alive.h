// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Overzombie.h"
#include "AI_Overzombie_Alive.generated.h"

class APawn_Zombie;

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API AAI_Overzombie_Alive : public AAI_Overzombie
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
		TArray<APawn_Zombie*> AllZombies_Hearing;
	UPROPERTY()
		TArray<APawn_Zombie*> AllZombies_NoiseNSmell;


		UPROPERTY()
			float Elevation = 130.f;
		UPROPERTY()
			float MeshPivotOffset = 75.f;
};
