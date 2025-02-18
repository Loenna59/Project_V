// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossLookOutState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAnimInstance.h"

void UBossLookOutState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->ChangeEyeColor(FLinearColor(1,0.844786,0.016844),40);
	Boss->GetBossAnimInstance()->OnPlayMontage(EBossMontage::Rage);
}

void UBossLookOutState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
}

void UBossLookOutState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
