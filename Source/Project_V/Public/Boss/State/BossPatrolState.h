// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBaseState.h"
#include "BossPatrolState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UBossPatrolState : public UBossBaseState
{
	GENERATED_BODY()
public:
	virtual void Enter(AThunderJaw* Boss, UThunderJawFSM* FSM) override;
	virtual void Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime) override;
	virtual void Exit(AThunderJaw* Boss, UThunderJawFSM* FSM) override;

	void Patrol(AThunderJaw* Boss, UThunderJawFSM* FSM);
	
public:
	float DistanceAlongSpline{0.0};

};
