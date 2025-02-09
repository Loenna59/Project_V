// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAIController.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AThunderJawAIController::AThunderJawAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AThunderJawAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThunderJawAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToTarget();
}

void AThunderJawAIController::MoveToTarget()
{
	auto* player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (player)
	{
		MoveToActor(player,100.0f);
	}
}

