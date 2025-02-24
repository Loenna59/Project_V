// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Arrow.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
	mesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapped);
}

void AArrow::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapped(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
