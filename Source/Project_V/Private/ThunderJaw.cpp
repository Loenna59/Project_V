// Fill out your copyright notice in the Description page of Project Settings.


#include "ThunderJaw.h"


// Sets default values
AThunderJaw::AThunderJaw()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AThunderJaw::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThunderJaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AThunderJaw::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

