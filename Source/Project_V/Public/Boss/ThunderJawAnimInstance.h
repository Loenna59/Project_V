// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
#pragma region PlayMontageFunc
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
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayDieMontage();
	UFUNCTION(BlueprintImplementableEvent, Category="BossAnim")
	void OnPlayRadarMontage();
#pragma endregion 

#pragma region AnimNotify
	UFUNCTION()
	void AnimNotify_PlayRageSound();
	UFUNCTION()
	void AnimNotify_PlayWalkSound();
	UFUNCTION()
	void AnimNotify_PlayChargeSound();
	UFUNCTION()
	void AnimNotify_PlayFallDownSound();
	UFUNCTION()
	void AnimNotify_PlayDieSound();
	UFUNCTION()
	void AnimNotify_PlayTailSound();
	UFUNCTION()
	void AnimNotify_MakeRadarEffect();
#pragma endregion
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	float BossAnimMoveSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	UAnimMontage* CurrentPlayingMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bDie{false};
};
