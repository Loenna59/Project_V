// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Arrow.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UParticleSystem> tmpFx(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/GoodFXImpact/FX/Particle/Chest_Point/Yellow/PS_GFXI_Yellow_Flare_04.PS_GFXI_Yellow_Flare_04'"));

	if (tmpFx.Succeeded())
	{
		hitFx = tmpFx.Object;
	}
}

void AArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitFx, Hit.Location);
}
   