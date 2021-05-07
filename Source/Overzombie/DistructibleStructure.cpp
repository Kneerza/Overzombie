// Fill out your copyright notice in the Description page of Project Settings.


#include "DistructibleStructure.h"
#include "Task.h"
//#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADistructibleStructure::ADistructibleStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");

	RootComponent = SceneComponent;

	Structure = CreateDefaultSubobject<UStaticMeshComponent>("Structure");
	//Structure->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, (TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"))));

	Structure->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Structure->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Collision");
	CollisionBox->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// --- CollisionProfile ---

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Structure->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//Structure->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	//Structure->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	// -------

	//Structure->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void ADistructibleStructure::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = HP;
	InitialRelativeRotation = Structure->GetRelativeRotation();

	auto AsTask = Cast<ATask>(GetParentActor());
	if (IsValid(AsTask))
	{
		UE_LOG(LogTemp, Error, TEXT("PARENT TASK: %s"), *AsTask->GetName())
			ParentTask = AsTask;
	}
}

// Called every frame
void ADistructibleStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHP < HP) {
		CurrentHP+= Constitution;
	}
}

void ADistructibleStructure::OnHit()
{
		CurrentHP--;

	if (CurrentHP <= 0) { Destruct(); return; }

	float bufHPFallingModifier = HP / CurrentHP;
	if (bufHPFallingModifier > 2)bufHPFallingModifier = 2;

	if (UKismetMathLibrary::Abs(Structure->GetRelativeRotation().Roll) >= MaxAmplitudeAngles)FallingDirection*=(-1);
	Structure->AddLocalRotation(FRotator(0.f, 0.f, FallingDirection*FallingAmplitudeModifier*(bufHPFallingModifier)));
}

void ADistructibleStructure::Destruct()
{
	OnDestructed();
	
	if (IsValid(ParentTask))
	{
		ParentTask->OnDestructed();
	}

	if (shouldBeDestroyedInBPOnly)return;

	if (OffsetWhenDestructed != FVector(0.f, 0.f, 0.f))
	{
		Structure->SetRelativeLocation(OffsetWhenDestructed);
	}
	Structure->SetRelativeRotation(DestroyedAngle);
	

	if (isDisablingCollisionWhenDestructed)	SetActorEnableCollision(false);

	CollisionBox->DestroyComponent();
	SetActorTickEnabled(false);

	auto Class = Structure->GetClass();
	auto StaticMeshes = GetComponentsByClass(Class);
	for (int32 i = 0; i < StaticMeshes.Num(); i++)
	{
		if (IsValid(StaticMeshes[i]))Cast<UStaticMeshComponent>(StaticMeshes[i])->SetCastShadow(false);
	}
	
}

void ADistructibleStructure::RecieveDamage(float DamageToRecieve)
{
	CurrentHP-= DamageToRecieve;

	if (CurrentHP <= 0) { Destruct(); return; }

	float bufHPFallingModifier = HP / CurrentHP;
	if (bufHPFallingModifier > 2)bufHPFallingModifier = 2;

	if (UKismetMathLibrary::Abs(Structure->GetRelativeRotation().Roll) >= MaxAmplitudeAngles)FallingDirection *= (-1);
	Structure->AddLocalRotation(FRotator(0.f, 0.f, FallingDirection*FallingAmplitudeModifier*(bufHPFallingModifier)));
}