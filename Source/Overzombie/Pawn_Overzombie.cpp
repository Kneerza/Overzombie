// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Overzombie.h"
#include "OverzombieController.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
APawn_Overzombie::APawn_Overzombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	//RootComponent = SceneComponent;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	
	RootComponent = Capsule;


	// --- CollisionProfile ---

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	// -------

	Capsule->SetConstraintMode(EDOFMode::XYPlane);
	Capsule->SetSimulatePhysics(true);

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Body->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, (TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"))));
	
	Body->AttachToComponent(Capsule, FAttachmentTransformRules::KeepRelativeTransform);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	
}

// Called when the game starts or when spawned
void APawn_Overzombie::BeginPlay()
{
	Super::BeginPlay();

	OverzombieController_Ref = Cast<AOverzombieController>(GetWorld()->GetFirstPlayerController());
	//Capsule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Capsule->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

// Called every frame
void APawn_Overzombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawn_Overzombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APawn_Overzombie::Initialize_BP()
{
	
	Body->SetRelativeLocation(FVector(0.f, 0.f, -100.f));

	Capsule->SetCapsuleRadius(45.f);
	Capsule->SetCapsuleHalfHeight(100.f);
}

void APawn_Overzombie::Kill()
{

	Destroy();
}