// Fill out your copyright notice in the Description page of Project Settings.


#include "OZPlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "OverzombieController.h"

// Sets default values
AOZPlayerPawn::AOZPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AOZPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	SpringArmComponent = FindComponentByClass<USpringArmComponent>();

	if (!IsValid(SpringArmComponent)) Destroy();
}

void AOZPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// --- Axis ---

	InputComponent->BindAxis(AOverzombieController::MoveLeftRight, this, &AOZPlayerPawn::MoveLeftRight_Handle);
	InputComponent->BindAxis(AOverzombieController::MoveUpDown, this, &AOZPlayerPawn::MoveUpDown_Handle);
	InputComponent->BindAxis(AOverzombieController::Zoom, this, &AOZPlayerPawn::Zoom_Handle);
}

void AOZPlayerPawn::MoveLeftRight_Handle(float AxisValue)
{
	if (AxisValue == 0)return;

	FVector NewLocation = GetActorLocation();
	NewLocation.Y += AxisValue * CameraMovementSpeed;

	SetActorLocation(NewLocation);
}

void AOZPlayerPawn::MoveUpDown_Handle(float AxisValue)
{
	if (AxisValue == 0)return;

	FVector NewLocation = GetActorLocation();
	NewLocation.X += AxisValue * CameraMovementSpeed;

	SetActorLocation(NewLocation);
}

void AOZPlayerPawn::Zoom_Handle(float AxisValue)
{
	auto Length = SpringArmComponent->TargetArmLength;

	if (Length < 100.f && AxisValue< 0)return;

	SpringArmComponent->TargetArmLength += AxisValue * ZoomSpeed;
}