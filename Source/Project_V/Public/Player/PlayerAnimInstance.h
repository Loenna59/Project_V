// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee,
	Base,
	Caster
};

UENUM(BlueprintType)
enum class EDamageDirection : uint8
{
	Front,
	Back,
	Left,
	Right
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHoldingKatana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator controlSpineRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float drawStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bTwiceShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool weaponChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType weaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool isDead;

	void SetWeaponAnim(UAnimInstance* anim);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayEquip();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartDodge();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(EDamageDirection direction);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLargeDamaged(float degrees);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartKatana();

	UFUNCTION(BlueprintImplementableEvent)
	void OnComboKatana(int32 index);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHideKatana();

	UFUNCTION()
	void OnDead();
	
	UFUNCTION()
	void OnFire(bool twice);

	UFUNCTION()
	void OnModifyWaistRotation(const FRotator& rotation);

	UFUNCTION()
	void OnSetDrawStrength(float strength);
};
