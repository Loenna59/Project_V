// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossDamageState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"

void UBossDamageState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);

	Boss->GetBossAIController()->StopMovement();
	if (Boss->bPartBroken)
	{
		Boss->GetBossAnimInstance()->OnPlayPartDestructionMontage();
		Boss->bPartBroken = false;

	}
	if (Boss->bTrapped)
	{
		Boss->BossTakeDamage(200.0f);
		Boss->GetBossAnimInstance()->OnPlayFallDownMontage();
		Boss->bTrapped = false;

	}
}

void UBossDamageState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	if (Boss->GetBossAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}
	else
	{
		Boss->GetBossAIController()->DetectedTarget = true;
		FSM->ChangeBossState(EBossState::Combat);
		Boss->bTrapped = false;
		Boss->bPartBroken = false;
	}
}

void UBossDamageState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
