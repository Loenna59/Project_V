// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ThunderJawAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAnimState : uint8
{
	None,
	Turn,
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
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	float BossAnimMoveSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	float BossAnimYaw;
};
