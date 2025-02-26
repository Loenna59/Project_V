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
	Base,
	Caster
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType weaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float damagedDegree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool damaged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool isDead;

	void SetWeaponAnim(UAnimInstance* anim);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayEquip();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartDodge();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float degrees);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLargeDamaged(float degrees);

	UFUNCTION()
	void OnDead();
	
	UFUNCTION()
	void OnFire(bool twice);

	UFUNCTION()
	void OnModifyWaistRotation(const FRotator& rotation);

	UFUNCTION()
	void OnSetDrawStrength(float strength);
};
