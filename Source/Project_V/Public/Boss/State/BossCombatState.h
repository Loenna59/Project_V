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
	Charge = 0,
	Tail,
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
	void RotateToTarget(AThunderJaw* Boss, float InterpSpeed);
	
	UFUNCTION()
	void Attack(AThunderJaw* Boss, float DeltaTime);
	UFUNCTION()
	void ChooseRandomPattern(AThunderJaw* Boss);
	UFUNCTION()
	void StartPattern(AThunderJaw* Boss);
	UFUNCTION()
	void PatternDelayEnd(AThunderJaw* Boss);
	
	// Melee Attack
	UFUNCTION()
	void Charge(AThunderJaw* Boss);

	// Range Attack
	UFUNCTION()
	void UseMachineGun(AThunderJaw* Boss);
	UFUNCTION()
	void UseDiscLauncher(AThunderJaw* Boss);
	UFUNCTION()
	void UseMouseLaser(AThunderJaw* Boss);

protected:
	// Pattern
	UPROPERTY()
	float PatternCurrentTime{0.0};
	UPROPERTY()
	float PatternTime{0.0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float PatternDelay{2.0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	bool bIsDelay{false};	

	UPROPERTY()
	FTimerHandle PatternTimerHandle;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	EAttackPattern UsingPattern;

	// Charge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float ChargePatternTime{2.0};
	// Tail
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float TailPatternTime{2.0};

	// Machine Gun
	UPROPERTY()
	FTimerHandle MachineGunTimer;
	UPROPERTY()
	float MachineGunDelayCurrentTime{0.0};
	UPROPERTY()
	float MachineGunDelay{0.1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float MachineGunPatternTime{3.0};
	// Disc Launcher
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float DiscLauncherPatternTime{3.0};
	// Mouse Laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float MouseLaserPatternTime{3.0};

};
