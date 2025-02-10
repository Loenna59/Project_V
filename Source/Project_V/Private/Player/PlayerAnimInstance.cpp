// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"

#include "Project_V.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayCharacter.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	player = CastChecked<APlayCharacter>(GetOwningActor());
	movementComponent = player->GetCharacterMovement();
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

		dodgeDirection = FVector(player->dodgeAxis.X, player->dodgeAxis.Y, 0);
		
		if (player->dodge)
		{
			isDodged = true;
		}
	}
}
