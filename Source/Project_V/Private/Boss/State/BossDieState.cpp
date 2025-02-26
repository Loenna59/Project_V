// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossDieState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"

void UBossDieState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->GetBossAIController()->StopMovement();
	Boss->GetBossAnimInstance()->OnPlayDieMontage();
	Boss->bDie = true;
	Boss->GetBossAnimInstance()->bDie = true;
}

void UBossDieState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
}

void UBossDieState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
