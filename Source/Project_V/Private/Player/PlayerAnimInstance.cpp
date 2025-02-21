// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"

#include "Project_V.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayCharacter.h"
#include "Player/PlayerWeapon.h"
#include "Player/WeaponAnim.h"

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
		// dodgeDirection = FTransform(player->GetControlRotation()).TransformVector(FVector(player->dodgeAxis.X, player->dodgeAxis.Y, 0));
		// dodgeDirection = FVector(player->dodgeAxis.X, player->dodgeAxis.Y, 0);
	
		// if (player->bIsDodge)
		// {
		// 	isDodged = true;
		// 	isAnchoredBow = false;
		// }
		// else
		// {
            isAnchoredBow = player->GetPlayerCameraMode() == EPlayerCameraMode::Anchored;
		// }

		if (!isAnchoredBow)
		{
			isShot = false;
		}

		drawStrength = player->GetDrawStrength();

		FRotator controlRotation = player->GetControlRotation().GetNormalized();

		controlSpineRotation = controlRotation;
		
		if (player->bIsShot)
		{
			isShot = true;
		}
		
		if (weaponAnim)
		{
			weaponAnim->bone = player->GetMesh()->GetBoneTransform(TEXT("index_03_r"));
			weaponAnim->bIsAnchored = !isShot && isAnchoredBow;
			// weaponAnim->bIsPlayingMontage = Montage_IsPlaying(player->equipWeaponMontage);
		}

	}
}
