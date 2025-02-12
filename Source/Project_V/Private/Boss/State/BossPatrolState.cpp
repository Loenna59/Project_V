// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossPatrolState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Kismet/KismetMathLibrary.h"

void UBossPatrolState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
}

void UBossPatrolState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	
	FVector bossPos = Boss->GetActorLocation();
	FVector patrolPointPos = FSM->PatrolPoints[FSM->CurrentTargetPoint];
	float dist = UKismetMathLibrary::Vector_Distance2D(bossPos,patrolPointPos);
	if (dist > 150.0f)
	{
		if (Boss->GetBossAIController())
		{
			//PrintLogFunc(TEXT("patrol %f"),dist);
			Boss->GetBossAIController()->MoveToLocation(patrolPointPos);
		}
	}
	else if (dist <= 150.0f)
	{
		FSM->ArrivedTargetPoint = true;
		FSM->ChangePatrolTargetPoint();
	}
	
}

void UBossPatrolState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
