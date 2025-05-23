// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossIdleState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"

void UBossIdleState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->GetEyeMatInst()->SetVectorParameterValue(FName("EyeColor"),FLinearColor(0,0.14,1));
	Boss->GetEyeMatInst()->SetScalarParameterValue(FName("EmissivePower"),100);
	Boss->GetBossAIController()->StopMovement();
}

void UBossIdleState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
}

void UBossIdleState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
