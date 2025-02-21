// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNodeReference.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_V_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class UCharacterMovementComponent* movementComponent;

	UPROPERTY()
	class UWeaponAnim* weaponAnim;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class APlayCharacter* player;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float walkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool shouldMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector dodgeDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDodged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isPlayingDodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isAnchoredBow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator controlSpineRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float drawStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isShot;

	void SetWeaponAnim(UAnimInstance* anim);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayEquip();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartDodge(FVector direction);
};
