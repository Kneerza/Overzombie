// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Alive_Civilian_Dummy.h"
#include "Pawn_Alive_Civilian.h"
#include "DistructibleStructure.h"

AAI_Alive_Civilian_Dummy::AAI_Alive_Civilian_Dummy()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAI_Alive_Civilian_Dummy::ActivateAI()
{
	UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"))

	auto AsDisStructure = Cast<ADistructibleStructure>(PossessedAliveCivilian_Ref->GetParentActor());
	if (IsValid(AsDisStructure))
	{
		UE_LOG(LogTemp, Error, TEXT("PARENT DisStructure: %s"), *AsDisStructure->GetName())
			ParentAsDisStructure = AsDisStructure;
	}
}

void AAI_Alive_Civilian_Dummy::ChangeBehavior(EBehaviorMode NewBehaviorMode)
{

}
void AAI_Alive_Civilian_Dummy::RunFromZombies()
{

}

void AAI_Alive_Civilian_Dummy::DamageParentDisStructure(float Damage)
{
	if (IsValid(ParentAsDisStructure))
	{
		ParentAsDisStructure->RecieveDamage(Damage);
	}
}