// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAIController.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawFSM.h"
#include "Boss/State/BossBaseState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
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
	if (!Boss)
	{
		return;
	}

	if (!Boss->bIsLevelSequenceEnd || Boss->bDie)
	{
		return;
	}

	if (bDebugMode)
	{
		DrawDebugSphere(GetWorld(),Boss->GetActorLocation(),CombatDist, 20,FColor::Red);
		DrawDebugSphere(GetWorld(),Boss->GetActorLocation(),DetectDist, 20,FColor::Green);
		DrawDebugSphere(GetWorld(),Boss->GetActorLocation(),Boss->MeleeAttackDist, 20,FColor::Yellow);
		DrawDebugSphere(GetWorld(),Boss->GetActorLocation(),Boss->RangeAttackDist, 20,FColor::Yellow);
	}
		

	if (DetectedTarget)
	{
		// target이 감지 됐을 때 타겟과의 거리를 업데이트 해줌
		UpdateDistanceFromTarget();

		// 타겟의 앞 뒤 구분을 위한 dot product 함수
		UpdateFacingDot();

		// 플레이어가 어느 범위에 있는지 확인
		EvaluateTargetDistance(DeltaTime);
		
		// stimulus age를 가져와서 target을 놓쳤는지 아닌지를 확인하는 코드
		CheckTargetThroughStimulus();
	}
}


void AThunderJawAIController::InitComponent()
{
	AIPC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPC"));
	if (!AIPC)
	{
		PRINTLOG(TEXT("AIPC is null"));
	}
		
	SetPerceptionComponent(*AIPC);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = DetectDist;
	SightConfig->LoseSightRadius = DetectDist + 1000;
	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->SetMaxAge(100.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	
	AIPC->ConfigureSense(*SightConfig);
	AIPC->SetDominantSense(SightConfig->GetSenseImplementation());
	DetectedTarget = false;
}

void AThunderJawAIController::MoveToPlayer()
{
	if (!Boss->GetAloy())
	{
		return;
	}
	MoveToActor(Boss->GetAloy(), 200.0f);
}

void AThunderJawAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	
	if (Boss->GetFSMComponent()->GetCurrentState()->BossState != EBossState::Patrol)
	{
		return;
	}
	
	if (Stimulus.Type== UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			auto* player = Boss->GetAloy();
			if (player)
			{
				PRINTLOG(TEXT("%s"),*Stimulus.GetDebugDescription());
				// 플레이어가 감지됨
				// 경계모드로 변환 된 상태이면 감지됐던 장소를 저장
				PRINTLOG(TEXT("Target is in Sight"));
				DetectedTarget = true;
				Boss->GetFSMComponent()->bIsArrivedDetectedLocation = false;
				DetectedLocation = player->GetActorLocation();
			}
		}
	}
}

void AThunderJawAIController::UpdateDistanceFromTarget()
{
	if (!Boss->GetAloy())
	{
		return;
	}
	
	FVector bossPos = Boss->GetActorLocation();
	FVector targetPos = Boss->GetAloy()->GetActorLocation();
	// 평지여서 2D로 해놓음, 나중에 평지가 아니게 되면 바꿔야함
	DistanceFromTarget = UKismetMathLibrary::Vector_Distance2D(bossPos,targetPos);
}

void AThunderJawAIController::UpdateFacingDot()
{
	if (!Boss->GetAloy())
	{
		return;
	}
	//PRINTLOGTOSCREEN(TEXT("FacingDot : %f"), FacingDot);
	FVector bossPos = Boss->GetActorLocation();
	FVector targetPos = Boss->GetAloy()->GetActorLocation();
	FVector direction = (targetPos - bossPos).GetSafeNormal();
	FacingDot = FVector::DotProduct(Boss->GetActorForwardVector(),direction);
}

void AThunderJawAIController::EvaluateTargetDistance(float DeltaTime)
{
	auto* bossFSM = Boss->GetFSMComponent();
	if (bossFSM->GetCurrentState()->BossState == EBossState::Combat || bossFSM->GetCurrentState()->BossState == EBossState::Damage)
	{
		return;
	}
	
	// 공격모드 거리까지 올 때
	if (DistanceFromTarget <= CombatDist)
	{
		LoseTargetTime = 0.0f;
		bossFSM->ChangeBossState(EBossState::Combat);
	}
	// 공격모드 거리보단 멀고 감지거리보다는 가까울 때
	else if (DistanceFromTarget > CombatDist && DistanceFromTarget < SightConfig->LoseSightRadius)
	{
		// Radar Pattern 생기면 실행할 부분
		//PRINTLOG(TEXT("LoseTargetTime %f"), LoseTargetTime);
		if (bossFSM->GetCurrentState()->BossState != EBossState::Combat)
		{
			LoseTargetTime += DeltaTime;
			bossFSM->ChangeBossState(EBossState::LookOut);
		}
	}
}

void AThunderJawAIController::CheckTargetThroughStimulus()
{
	if (!Boss->GetAloy())
	{
		return;
	}
	
	FActorPerceptionBlueprintInfo Info;
	AIPC->GetActorsPerception(Boss->GetAloy(), Info);
	
	for (const auto& Stimulus : Info.LastSensedStimuli)
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			//PRINTLOG(TEXT("%f"),Stimulus.GetAge());
			if (Stimulus.GetAge() > SightConfig->GetMaxAge() ||
				LoseTargetTime > SightConfig->GetMaxAge())
			{
				LoseTargetTime = 0.0f;
				DetectedTarget = false;
				Boss->GetFSMComponent()->ChangeBossState(EBossState::Patrol);
			}
		}
	}
}


