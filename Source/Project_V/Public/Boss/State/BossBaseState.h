// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Project_V.h"
#include "Boss/ThunderJawFSM.h"
#include "UObject/Object.h"
#include "BossBaseState.generated.h"

class UThunderJawFSM;
class AThunderJaw;

UCLASS()
class PROJECT_V_API UBossBaseState : public UObject
{
	GENERATED_BODY()
public:
	virtual void Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
	{
		FString str = UEnum::GetValueAsString(BossState);
		PRINTLOG(TEXT("Enter State %s"), *str);
	};
	virtual void Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime) {};
	virtual void Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
	{
		FString str = UEnum::GetValueAsString(BossState);
		PRINTLOG(TEXT("Exit State %s"), *str);
	};

public:
	EBossState BossState;
	
};
