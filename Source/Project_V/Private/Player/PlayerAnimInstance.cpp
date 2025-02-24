// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayCharacter.h"
#include "Player/Weapon/WeaponAnim.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	player = CastChecked<APlayCharacter>(GetOwningActor());
	movementComponent = player->GetCharacterMovement();
}

void UPlayerAnimInstance::SetWeaponAnim(UAnimInstance* anim)
{
	weaponAnim = CastChecked<UWeaponAnim>(anim);
}

void UPlayerAnimInstance::OnFire(bool twice)
{
	isShot = true;
	bTwiceShot = twice;
}

void UPlayerAnimInstance::OnModifyWaistRotation(const FRotator& rotation)
{
	controlSpineRotation = rotation;
}

void UPlayerAnimInstance::OnSetDrawStrength(float strength)
{
	drawStrength = strength;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (movementComponent)
	{
		velocity = movementComponent->Velocity;
		walkSpeed = movementComponent->MaxWalkSpeed;

		FVector acceleration = movementComponent->GetCurrentAcceleration();
		
		shouldMove = acceleration != FVector::ZeroVector && walkSpeed > 3.f;

		isFalling = movementComponent->IsFalling();

	}

	if (player)
	{
		isAnchoredBow = player->GetPlayerCameraMode() == EPlayerCameraMode::Anchored;

		if (!isAnchoredBow)
		{
			isShot = false;
		}
		
		if (weaponAnim)
		{
			weaponAnim->bone = player->GetMesh()->GetBoneTransform(TEXT("index_03_r"));
			weaponAnim->bIsAnchored = !isShot && isAnchoredBow;
		}

	}
}
