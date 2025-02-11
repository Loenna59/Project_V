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

		// dodgeDirection = FTransform(player->GetControlRotation()).TransformVector(FVector(player->dodgeAxis.X, player->dodgeAxis.Y, 0));
		dodgeDirection = FVector(player->dodgeAxis.X, player->dodgeAxis.Y, 0);
		
		if (player->bIsDodge)
		{
			isDodged = true;
		}

		if (isPlayingDodge)
		{
			OnMoveDodge();
		}
	}
}

void UPlayerAnimInstance::OnMoveDodge()
{
	dodgeDirection = FTransform(player->GetControlRotation()).TransformVector(dodgeDirection).GetSafeNormal();
	
	FVector p0 = player->GetActorLocation();
	FVector vt = player->dodgeSpeed * GetWorld()->DeltaTimeSeconds * dodgeDirection;

	FVector forwardVector = player->GetActorForwardVector();

	float dotProduct = FVector::DotProduct(forwardVector, dodgeDirection);
	float radians = FMath::Acos(dotProduct);
	float degrees = FMath::RadiansToDegrees(radians);

	if (degrees > 1.f)
	{
		FRotator targetRotation = FRotationMatrix::MakeFromX(dodgeDirection).Rotator();
		FRotator r0 = player->GetControlRotation();

		FRotator newRotation = FMath::RInterpTo(r0, targetRotation, GetWorld()->DeltaTimeSeconds, 360.f);
		player->SetActorRotation(newRotation);
	}

	player->SetActorLocation(p0 + vt);
}
