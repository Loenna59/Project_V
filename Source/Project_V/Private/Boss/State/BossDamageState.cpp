// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossDamageState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAnimInstance.h"

void UBossDamageState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->GetBossAnimInstance()->OnPlayPartDestructionMontage();
}

void UBossDamageState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	if (!Boss->GetBossAnimInstance()->IsAnyMontagePlaying())
	{
		FSM->ChangeBossState(FSM->GetPrevState()->BossState);		
	}
}

void UBossDamageState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
