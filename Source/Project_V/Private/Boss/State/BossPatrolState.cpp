// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossPatrolState.h"

#include "Project_V.h"

void UBossPatrolState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
}

void UBossPatrolState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	PRINTLOGTOSCREEN(TEXT("patrol state update"));
}

void UBossPatrolState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}


