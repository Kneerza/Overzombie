// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PsychoSphere.generated.h"

class USphereComponent;
class AAI_Zombie;

UCLASS()
class OVERZOMBIE_API APsychoSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	APsychoSphere();

	// --- Components ---

	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionSphere;
	// ------

	UPROPERTY()
		TArray<AAI_Zombie*> ControlledZombies;

	UPROPERTY(EditAnywhere)
		float Radius = 3000.f;

	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap(AActor* OverlappingActor);
	UFUNCTION(BlueprintCallable)
		void OnEndOverlap(AActor* OverlappingActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
