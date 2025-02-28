// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBaseState.h"
#include "BossCombatState.generated.h"

/**
 * 
 */
UENUM()
enum class EAttackPattern : uint8
{
	None = 0,
	Charge,
	Tail,
	MachineGun,
	DiscLauncher,
	MouseLaser,
	ChasePlayer,
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
	void InitComponents(AThunderJaw* Boss);
	
#pragma region AttackFunc
	UFUNCTION()
	void Attack(AThunderJaw* Boss);
	UFUNCTION()
	void MakeTraceBoxAndCheckHit(FVector start, FVector end, FVector boxHalfSize, AThunderJaw* Boss);
#pragma endregion

#pragma region ChoosePatternFunc
	UFUNCTION()
	void StartChoosingPatternCycle(AThunderJaw* Boss);
	UFUNCTION()
	void ChoosePattern(AThunderJaw* Boss);
	UFUNCTION()
	int32 MakeRandomRangeNum(AThunderJaw* Boss);
#pragma endregion 
	
#pragma region MeleeFunc
	UFUNCTION()
	void Charge(AThunderJaw* Boss);
	UFUNCTION()
	void Tail(AThunderJaw* Boss);
#pragma endregion
	
#pragma region RangeFunc
	UFUNCTION()
	void MachineGun(AThunderJaw* Boss);
	UFUNCTION()
	void DiscLauncher(AThunderJaw* Boss);
	UFUNCTION()
	void MouseLaser(AThunderJaw* Boss);
#pragma endregion
	
	UFUNCTION()
	void ChasePlayer(AThunderJaw* Boss);

protected:
	UPROPERTY()
	bool bIsRotateBody{false};
	
#pragma region BasePatternVariance
	UPROPERTY()
	float PatternCurrentTime{0.0};
	UPROPERTY()
	float PatternTime{0.0};
	UPROPERTY()
	float PatternDelay{2.0};
	UPROPERTY()
	bool bIsDelay{false};	
	UPROPERTY()
	bool bMeleeHit{false};

	UPROPERTY()
	FTimerHandle PatternTimerHandle;	
	UPROPERTY()
	EAttackPattern UsingPattern;
#pragma endregion 

#pragma region ChargePatternVariance
	UPROPERTY()
	float recoilTime{1.5};
	UPROPERTY()
	float ChargeTime{2.0};
	UPROPERTY()
	bool ChargeFlag{false};
	UPROPERTY()
	bool ChargeStart{false};
	UPROPERTY()
	FVector PerposeLocation;
	UPROPERTY()
	FTimerHandle ChargeTimerHandle;
	UPROPERTY(EditAnywhere)
	float ChargeDamage{30.0f};
#pragma endregion 

#pragma region TailPatternVariance
	UPROPERTY()
	float TailPatternTime{1.0};
	UPROPERTY(EditAnywhere)
	float TailDamage{30.0f};
#pragma endregion

#pragma region MachineGunPatternVariance
	UPROPERTY()
	float MachineGunPatternTime{3.0};
	UPROPERTY()
	float MachineGunDelayCurrentTime{0.0};
	UPROPERTY()
	float MachineGunDelay{0.1};
#pragma endregion

#pragma region DiscLauncherPatternVariance
	UPROPERTY()
	float DiscLauncherPatternTime{3.5};
	UPROPERTY()
	float DiscLauncherDelayCurrentTime{0.0};
	UPROPERTY()
	float DiscLauncherDelay{1.0};
#pragma endregion
	
	// Mouse Laser
	UPROPERTY()
	float MouseLaserPatternTime{3.0};
};
