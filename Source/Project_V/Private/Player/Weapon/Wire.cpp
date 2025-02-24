// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Wire.h"
#include "CableComponent.h"

// Sets default values
AWire::AWire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	cableComp = CreateDefaultSubobject<UCableComponent>(TEXT("CableComp"));
	cableComp->SetupAttachment(mesh);

	cableComp->SetRelativeLocation(FVector(-38, 0, 0));
	cableComp->bAttachEnd = false;
	cableComp->CableWidth = 3;
}

// Called when the game starts or when spawned
void AWire::BeginPlay()
{
	Super::BeginPlay();
	
}

