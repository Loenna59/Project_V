// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/DiscMissile.h"

#include "Project_V.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/PlayCharacter.h"


// Sets default values
ADiscMissile::ADiscMissile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitComponents();
}

// Called when the game starts or when spawned
void ADiscMissile::BeginPlay()
{
	Super::BeginPlay();

	Root->OnComponentBeginOverlap.AddDynamic(this,&ADiscMissile::OnDiscMissileBeginOverlap);
}

// Called every frame
void ADiscMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADiscMissile::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
		Root->SetBoxExtent(FVector(30.000000,10.000000,10.000000));
		Root->SetGenerateOverlapEvents(true);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeLocation(FVector(-30,0,0));
		Mesh->SetRelativeRotation(FRotator(-90,0,0));
		Mesh->SetRelativeScale3D(FVector(0.15,0.15,0.75));
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("'/Game/Blueprints/Boss/Material/M_MachineGunBullet.M_MachineGunBullet'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0,tempMat.Object);
	}
	
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMC"));
	if (PMC)
	{
		PMC->SetUpdatedComponent(Root);
		PMC->InitialSpeed = 5000.0f;
		PMC->MaxSpeed = 5000.0f;
		PMC->bRotationFollowsVelocity = true;
		PMC->bShouldBounce = true;
		PMC->Bounciness = 0.3f;
		PMC->ProjectileGravityScale = 1.0f;
	}
}

void ADiscMissile::OnDiscMissileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* player = Cast<APlayCharacter>(OtherActor);
	if (player)
	{
		PRINTLOG(TEXT("hit missile"));
		Destroy();
	}
}

