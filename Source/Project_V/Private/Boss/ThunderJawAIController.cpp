// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAIController.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawFSM.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/PlayCharacter.h"


// Sets default values
AThunderJawAIController::AThunderJawAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	InitComponent();
}

// Called when the game starts or when spawned
void AThunderJawAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIPC)
	{
		AIPC->OnTargetPerceptionUpdated.AddDynamic(this,&AThunderJawAIController::TargetPerceptionUpdated);
	}

	// pawn이 possess되는건 생성 후 시점이기때문에 beginplay에서 초기화 해야한다.
	Boss = Cast<AThunderJaw>(GetPawn());
}

// Called every frame
void AThunderJawAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DetectedTarget)
	{
		// target이 감지 됐을 때 타겟과의 거리를 업데이트 해줌
		FVector bossPos = Boss->GetActorLocation();
		FVector targetPos = Boss->GetAloy()->GetActorLocation();
		DistanceFromTarget = UKismetMathLibrary::Vector_Distance2D(bossPos,targetPos);
		DrawDebugLine(GetWorld(),bossPos,targetPos,FColor::Red,false,0.1f,0,2);
		PrintLogFunc(TEXT("%f"),DistanceFromTarget);

		 if (DistanceFromTarget < SightConfig->SightRadius * 0.7)
		 {
		 	Boss->GetFSMComponent()->ChangeBossState(EBossState::Combat);
		 }

		// stimulus age를 가져와서 target을 놓쳤는지 아닌지를 확인하는 코드
		FActorPerceptionBlueprintInfo Info;
		AIPC->GetActorsPerception(Boss->GetAloy(), Info);
		for (const auto& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
			{
				UE_LOG(LogTemp,Warning,TEXT("%f"),Stimulus.GetAge());
				if (Stimulus.GetAge() > SightConfig->GetMaxAge())
				{
					DetectedTarget = false;
					Boss->GetFSMComponent()->ChangeBossState(EBossState::Patrol);
				}
			}
		}
	}
}


void AThunderJawAIController::InitComponent()
{
	AIPC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPC"));
	if (!AIPC)
	{
		UE_LOG(LogTemp,Warning,TEXT("AIPC is null"));
	}
		
	SetPerceptionComponent(*AIPC);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->SetMaxAge(3.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	AIPC->ConfigureSense(*SightConfig);
	AIPC->SetDominantSense(SightConfig->GetSenseImplementation());


	DetectedTarget = false;
}

void AThunderJawAIController::MoveToPlayer()
{
	MoveToActor(Boss->GetAloy(), 200.0f);
}

void AThunderJawAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type== UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			PrintLogFunc(TEXT("find target"));
			DetectedTarget = true;
		}
	}
}




