// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossCombatState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayCharacter.h"


void UBossCombatState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
}

void UBossCombatState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	// 플레이어를 감지계속 하고 있는 상태이면 몸을 플레이어 쪽으로 계속 돌려준다.
	if (Boss->GetBossAIController()->FacingDot > 0)
	{
		// 플레이어가 전방에 있음, 거리에 따른 공격 실행
		
		//UE_LOG(LogTemp,Warning,TEXT("attack"));
		float dist = Boss->GetBossAIController()->DistanceFromTarget; 
		if (dist <= Boss->MeleeAttackDist)
		{
			//UE_LOG(LogTemp,Warning,TEXT("boss melee attack"));
		}
		else if (dist > Boss->MeleeAttackDist)
		{
			//UE_LOG(LogTemp,Warning,TEXT("boss range attack"));
		}
	}
	else
	{
		FRotator targetRotate = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(),Boss->GetAloy()->GetActorLocation());
		FRotator newRotation = UKismetMathLibrary::RInterpTo(Boss->GetActorRotation(),targetRotate, DeltaTime, 0.7f);
		Boss->SetActorRotation(newRotation);
	}
}

void UBossCombatState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
