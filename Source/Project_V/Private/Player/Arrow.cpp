// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Arrow.h"

#include "Project_V.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(mesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tmpMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Ranged/SM_ProjectileArrowMesh.SM_ProjectileArrowMesh'"));

	if (tmpMesh.Succeeded())
	{
		mesh->SetStaticMesh(tmpMesh.Object);
		mesh->SetCollisionProfileName(TEXT("PlayerProjectile"));
	}

	projectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	projectileMovementComp->SetUpdatedComponent(RootComponent);
	projectileMovementComp->InitialSpeed = 10000;
	projectileMovementComp->MaxSpeed = 10000;
	projectileMovementComp->ProjectileGravityScale = 1;
	projectileMovementComp->bShouldBounce = false;

	projectileMovementComp->bSimulationEnabled = false;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AArrow::Fire()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	// ProjectileMovementComponent 활성화
	projectileMovementComp->SetActive(true);
	projectileMovementComp->Activate(true);
	projectileMovementComp->SetUpdatedComponent(RootComponent);
	projectileMovementComp->bSimulationEnabled = true;


	// 물리 속성 설정
	projectileMovementComp->ProjectileGravityScale = 1;
	projectileMovementComp->InitialSpeed = 10000;
	projectileMovementComp->MaxSpeed = 10000;

	// Velocity 설정
	projectileMovementComp->Velocity = FVector(1, 0, 0) * projectileMovementComp->InitialSpeed;

	PrintLogFunc(TEXT("%f"), mesh->GetForwardVector());
}

