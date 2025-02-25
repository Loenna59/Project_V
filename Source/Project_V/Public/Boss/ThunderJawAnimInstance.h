// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThunderJawFSM.h"
#include "Animation/AnimInstance.h"
#include "ThunderJawAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBossMontage : uint8
{
	None,
	Turn,
	Rage,
	Charge,
	Tail
};
UCLASS()
class PROJECT_V_API UThunderJawAnimInstance : public UAnimInstance
{
	GENERATED_BODY() 
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayTurnMontage();
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayRageMontage();
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayChargeMontage();
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayTailMontage();
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayPartDestructionMontage();	
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayFallDownMontage();
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	float BossAnimMoveSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	UAnimMontage* CurrentPlayingMontage;
};
