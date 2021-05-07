// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Overzombie.h"
#include "OverzombieController.h"


void AAI_Overzombie::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OverzombieController_Ref = Cast<AOverzombieController>(GetWorld()->GetFirstPlayerController());
	TaskManagerComponent = OverzombieController_Ref->TaskManagerComponent;
}

void AAI_Overzombie::ChangeBehavior(EBehaviorMode NewBehaviorMode)
{
	/*
	//if (NewBehaviorMode == CurrentBehaviorMode)return;
	CurrentBehaviorMode = NewBehaviorMode;
	switch (CurrentBehaviorMode)
	{
	case EBehaviorMode::Idle:

		

		break;

	default:
		break;
	}
	*/
}

