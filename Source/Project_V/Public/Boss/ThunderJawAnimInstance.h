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
	UFUNCTION()
	void PlayMontage(UAnimMontage* montage);
	UFUNCTION()
	void OnAnimEnded(UAnimMontage* Montage, bool bInterrupted);
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	float BossAnimMoveSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossAnim")
	float BossAnimYaw;
	UPROPERTY()
	bool bMontagePlaying = false;
	

	UPROPERTY()
	TMap<EAnimState, class UAnimMontage*> MontageMap;
	UPROPERTY()
	class UAnimMontage* CurrentMontage;
};
