// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OZPlayerPawn.generated.h"

class USpringArmComponent;

UCLASS()
class OVERZOMBIE_API AOZPlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	AOZPlayerPawn();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	float CameraMovementSpeed = 120.f;
	float ZoomSpeed = 120.f;

	UPROPERTY()
	USpringArmComponent* SpringArmComponent;

	void MoveLeftRight_Handle(float AxisValue);
	void MoveUpDown_Handle(float AxisValue);
	void Zoom_Handle(float AxisValue);
};
