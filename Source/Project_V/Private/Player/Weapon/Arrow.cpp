// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Arrow.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AArrow::DestroyAfterPlayFX()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitFx, GetActorLocation());
	
	FTimerHandle timerHandle;
	
	TWeakObjectPtr<AArrow> weakThis = this;

	GetWorld()->GetTimerManager()
	.SetTimer(
		timerHandle,
		[weakThis] ()
		{
			if (weakThis.IsValid())
			{
				weakThis->Destroy();
			}
		},
		1.f,
		false
	);
}
   