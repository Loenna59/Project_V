// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBaseState.h"
#include "BossCombatState.generated.h"

/**
 * 
 */
UENUM()
enum class ERangeWeapon : uint8
{
	MachineGun,
	DiscLauncher,
	MouseLaser,
};
UCLASS()
class PROJECT_V_API UBossCombatState : public UBossBaseState
{
	GENERATED_BODY()
public:
	virtual void Enter(AThunderJaw* Boss, UThunderJawFSM* FSM) override;
	virtual void Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime) override;
	virtual void Exit(AThunderJaw* Boss, UThunderJawFSM* FSM) override;
	
protected:
	UFUNCTION()
	void RangeAttack(AThunderJaw* Boss, float DeltaTime);
	UFUNCTION()
	void ChooseRandomRangeAttack(AThunderJaw* Boss);
	UFUNCTION()
	void UseMachineGun(AThunderJaw* Boss);
	UFUNCTION()
	void UseDiscLauncher(AThunderJaw* Boss);
	UFUNCTION()
	void UseMouseLaser(AThunderJaw* Boss);

protected:
	UPROPERTY()
	FTimerHandle MachineGunTimer;
	UPROPERTY()
	float PatternCurrentTime{0.0};
	UPROPERTY()
	float PatternTime{0.0};

	ERangeWeapon UsingWeapon;

	// machine gun
	UPROPERTY()
	float MachineGunDelayCurrentTime{0.0};
	UPROPERTY()
	float MachineGunDelay{0.2};
	UPROPERTY()
	float MachineGunPatternTime{3.0};

	UPROPERTY()
	float DiscLauncherPatternTime{3.0};

	UPROPERTY()
	float MouseLaserPatternTime{3.0};

};
