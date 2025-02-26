// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/PlayerProjectile.h"

#include "Project_V.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"


// Sets default values
APlayerProjectile::APlayerProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(mesh);

	moveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	moveComp->SetUpdatedComponent(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tmpMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Ranged/SM_ProjectileArrowMesh.SM_ProjectileArrowMesh'"));

	if (tmpMesh.Succeeded())
	{
		mesh->SetStaticMesh(tmpMesh.Object);
		mesh->SetCollisionProfileName(TEXT("PlayerProjectile"));
	}

	// 물리 속성 설정
	moveComp->ProjectileGravityScale = 1;
	moveComp->bShouldBounce = false;
	moveComp->bAutoActivate = false;
	moveComp->bRotationFollowsVelocity = true;

	moveComp->SetActive(false);

	ConstructorHelpers::FObjectFinder<UParticleSystem> tmpEffect(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Medieval_Weapons/VFX/P_ArrowTrail.P_ArrowTrail'"));

	if (tmpEffect.Succeeded())
	{
		tailVFX = tmpEffect.Object;
	}
}

// Called when the game starts or when spawned
void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	moveComp->InitialSpeed = initialSpeed;
	moveComp->MaxSpeed = maxSpeed;

	mesh->OnComponentHit.AddDynamic(this, &APlayerProjectile::OnComponentHit);
	mesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerProjectile::OnOverlapped);
}

void APlayerProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bOrtho)
	{
		SetActorRelativeRotation(Hit.ImpactNormal.Rotation() - FRotator(180, 0, 0));
		onEventComponentHit.Broadcast();
	}
	
}

void APlayerProjectile::Fire(FVector to, float alpha, bool ortho)
{
	bOrtho = ortho;
	
	FVector direction = (to - GetActorLocation()).GetSafeNormal();
	
	FVector currentVelocity = direction * initialSpeed * FMath::Clamp(alpha, 0, 1);
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	moveComp->Velocity = currentVelocity;
	moveComp->SetActive(true, true);

	UGameplayStatics::SpawnEmitterAttached(tailVFX, mesh);
}

