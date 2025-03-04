// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBaseState.h"
#include "BossLookOutState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UBossLookOutState : public UBossBaseState
{
	GENERATED_BODY()

public:
	virtual void Enter(AThunderJaw* Boss, UThunderJawFSM* FSM) override;
	virtual void Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime) override;
	virtual void Exit(AThunderJaw* Boss, UThunderJawFSM* FSM) override;

	void PatrolToDetectLocation(AThunderJaw* Boss, UThunderJawFSM* FSM);
	void Radar(AThunderJaw* Boss, UThunderJawFSM* FSM);
	void MakeSphereTrace(AThunderJaw* Boss, UThunderJawFSM* FSM);
public:
	UPROPERTY()
	bool bIsArrivedDetectedArea{false};
	UPROPERTY()
	FTimerHandle RadarTimerHandle;
	UPROPERTY()
	bool bUsingRadar{false};
	UPROPERTY()
	float RadarUsingTime{20.0f};
	UPROPERTY()
	FTimerHandle SencingTimerHandle;
	UPROPERTY()
	float SencingTime{5.0f};
	UPROPERTY()
	bool bOnSphereTrace{false};
	
};
