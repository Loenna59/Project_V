// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAIController.h"

#include "Project_V.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Project_V/Project_VCharacter.h"


// Sets default values
AThunderJawAIController::AThunderJawAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIPC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPC"));
	SetPerceptionComponent(*AIPC);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	
	AIPC->ConfigureSense(*SightConfig);
	AIPC->SetDominantSense(SightConfig->GetSenseImplementation());
}

// Called when the game starts or when spawned
void AThunderJawAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIPC)
	{
		//AIPC->OnPerceptionUpdated.AddDynamic(this,&AThunderJawAIController::PerceptionUpdated);
		AIPC->OnTargetPerceptionUpdated.AddDynamic(this,&AThunderJawAIController::TargetPerceptionUpdated);
	}
}

// Called every frame
void AThunderJawAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OnDetected)
	{
		MoveToPlayer();
	}
}

void AThunderJawAIController::MoveToPlayer()
{
	auto* player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (player)
	{
		MoveToActor(player,100.0f);
	}
}

void AThunderJawAIController::PerceptionUpdated(const TArray<AActor*>& Actors)
{
	// for (auto* actor : Actors)
	// {
	// 	auto* player = Cast<AProject_VCharacter>(actor);
	// 	if (player)
	// 	{
	// 		UE_LOG(LogTemp,Warning,TEXT("Hello"));
	// 		OnDetected = true;
	// 	}
	// }
}

void AThunderJawAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		PrintLogFunc(TEXT("find target"));
		MoveToPlayer();
	}
	else
	{
		PrintLogFunc(TEXT("Forgotten Target"));
	}
}




