// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Arrow.h"

#include "Boss/ThunderJaw.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	hitCollision->SetupAttachment(RootComponent);
	hitCollision->SetSphereRadius(3);
	hitCollision->SetRelativeLocation(FVector(45, 0,0 ));
	hitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AArrow::DestroyAfterPlayFX()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitFx, GetActorLocation());
	Destroy();
}

void AArrow::Fire(FVector to, float alpha, bool ortho)
{
	hitCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	hitCollision->SetCollisionProfileName("PlayerProjectile");
	Super::Fire(to, alpha, ortho);
}
   