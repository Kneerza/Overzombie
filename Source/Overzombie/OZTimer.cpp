// Fill out your copyright notice in the Description page of Project Settings.


#include "OZTimer.h"
#include "DistructibleStructure.h"

// Sets default values for this component's properties
UOZTimer::UOZTimer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOZTimer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	OwningDistructableStructure = Cast<ADistructibleStructure>(GetOwner());

	CreationTime = GetWorld()->GetTimeSeconds();
}


// Called every frame
void UOZTimer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if ((GetWorld()->GetTimeSeconds() - CreationTime) >= Lifetime)
	{
		if(IsValid(OwningDistructableStructure))OwningDistructableStructure->OnTimerGoesOff();
		
		DestroyComponent();
	}
}

