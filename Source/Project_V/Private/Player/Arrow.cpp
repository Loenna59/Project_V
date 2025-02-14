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

    // 물리 속성 설정
    projectileMovementComp->ProjectileGravityScale = 1;
    projectileMovementComp->InitialSpeed = initialSpeed;
    projectileMovementComp->MaxSpeed = maxSpeed;
    projectileMovementComp->bShouldBounce = false;
	projectileMovementComp->bAutoActivate = false;
	projectileMovementComp->bRotationFollowsVelocity = true;

	projectileMovementComp->SetActive(false);
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AArrow::Fire(float alpha)
{
	FVector currentVelocity = GetActorForwardVector() * initialSpeed * FMath::Clamp(alpha, 0, 1);
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	projectileMovementComp->Velocity = currentVelocity;
	projectileMovementComp->SetActive(true);
	projectileMovementComp->Activate();
}

