// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAIController.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawFSM.h"
#include "Boss/State/BossBaseState.h"
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

		// target이 앞에있는지 뒤에 있는지 알기위한 내적값
		FVector direction = (targetPos - bossPos).GetSafeNormal();
		FacingDot = FVector::DotProduct(Boss->GetActorForwardVector(),direction);
		//UE_LOG(LogTemp,Warning,TEXT("dotproduct : %f"),FacingDot);

		// 공격모드 거리까지 올 때
		 if (DistanceFromTarget <= Boss->DetectDist)
		 {
		 	LoseTargetTime = 0.0f;
		 	if (Boss->GetFSMComponent()->GetCurrentState()->currentStateEnum != EBossState::Combat)
		 	{
		 		Boss->GetFSMComponent()->ChangeBossState(EBossState::Combat);
		 	}
		 }
		// 공격모드 거리보단 멀고 감지거리보다는 가까울 때
		else if (DistanceFromTarget > Boss->DetectDist && DistanceFromTarget < SightConfig->LoseSightRadius)
		{
			LoseTargetTime += DeltaTime;
		}
		
		// stimulus age를 가져와서 target을 놓쳤는지 아닌지를 확인하는 코드
		FActorPerceptionBlueprintInfo Info;
		AIPC->GetActorsPerception(Boss->GetAloy(), Info);
		for (const auto& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
			{
				if (Stimulus.GetAge() > SightConfig->GetMaxAge() || LoseTargetTime > SightConfig->GetMaxAge())
				{
					LoseTargetTime = 0.0f;
					DetectedTarget = false;
					if (Boss->GetFSMComponent()->GetCurrentState()->currentStateEnum != EBossState::Patrol)
					{
						Boss->GetFSMComponent()->ChangeBossState(EBossState::Patrol);
					}
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
	SightConfig->SetMaxAge(10.0f);
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




