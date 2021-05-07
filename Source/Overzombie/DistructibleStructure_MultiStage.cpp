// Fill out your copyright notice in the Description page of Project Settings.


#include "DistructibleStructure_MultiStage.h"
#include "Task.h"
#include "Kismet/KismetMathLibrary.h"

void ADistructibleStructure_MultiStage::OnHit()
{
	CurrentHP--;

	if (CurrentHP <= 0) 
	{ Destruct(); return; }
	else if(CurrentHP <= HP/2 && !isDamaged)
	{ Damage(); return;}


	float bufHPFallingModifier = HP / CurrentHP;
	if (bufHPFallingModifier > 2)bufHPFallingModifier = 2;

	if (UKismetMathLibrary::Abs(Structure->GetRelativeRotation().Roll) >= MaxAmplitudeAngles)FallingDirection *= (-1);
	Structure->AddLocalRotation(FRotator(0.f, 0.f, FallingDirection*FallingAmplitudeModifier*(bufHPFallingModifier)));
}

void ADistructibleStructure_MultiStage::Damage()
{
	isDamaged = true;

	OnDamaged();

	if (IsValid(ParentTask))
	{
		ParentTask->OnDamaged();
	}
}

void ADistructibleStructure_MultiStage::RecieveDamage(float DamageToRecieve)
{
	CurrentHP -= DamageToRecieve;

	if (CurrentHP <= 0) { Destruct(); return; }
	else if (CurrentHP <= HP / 2 && !isDamaged)
	{
		Damage(); return;
	}

	float bufHPFallingModifier = HP / CurrentHP;
	if (bufHPFallingModifier > 2)bufHPFallingModifier = 2;

	if (UKismetMathLibrary::Abs(Structure->GetRelativeRotation().Roll) >= MaxAmplitudeAngles)FallingDirection *= (-1);
	Structure->AddLocalRotation(FRotator(0.f, 0.f, FallingDirection*FallingAmplitudeModifier*(bufHPFallingModifier)));
}