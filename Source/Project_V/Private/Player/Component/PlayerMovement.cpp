// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/PlayerMovement.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/PlayCharacter.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCameraMode.h"

// Sets default values for this component's properties
UPlayerMovement::UPlayerMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_move(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerMove.IA_PlayerMove'"));

	if (tmp_ia_move.Succeeded())
	{
		ia_move = tmp_ia_move.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_rotate(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerRotate.IA_PlayerRotate'"));

	if (tmp_ia_rotate.Succeeded())
	{
		ia_rotate = tmp_ia_rotate.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_jump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerJump.IA_PlayerJump'"));

	if (tmp_ia_jump.Succeeded())
	{
		ia_jump = tmp_ia_jump.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_sprint(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerSprint.IA_PlayerSprint'"));

	if (tmp_ia_sprint.Succeeded())
	{
		ia_shift = tmp_ia_sprint.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_move_pressed(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerMovePressed.IA_PlayerMovePressed'"));

	if (tmp_ia_move_pressed.Succeeded())
	{
		ia_movePressed = tmp_ia_move_pressed.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_doubleTap(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerDoubleTap.IA_PlayerDoubleTap'"));

	if (tmp_ia_doubleTap.Succeeded())
	{
		ia_doubleTap = tmp_ia_doubleTap.Object;
	}
}


// Called when the game starts
void UPlayerMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	movementComp = me->GetCharacterMovement();
}

void UPlayerMovement::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsPlayingDodge)
	{
		PlayingDodge();
	}
}

void UPlayerMovement::SetupInputBinding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBinding(input);

	input->BindAction(ia_move, ETriggerEvent::Triggered, this, &UPlayerMovement::Move);
	input->BindAction(ia_rotate, ETriggerEvent::Triggered, this, &UPlayerMovement::Rotate);
	input->BindAction(ia_jump, ETriggerEvent::Started, this, &UPlayerMovement::ActionJump);
	input->BindAction(ia_shift, ETriggerEvent::Triggered, this, &UPlayerMovement::OnTriggerShift);
	input->BindAction(ia_shift, ETriggerEvent::Completed, this, &UPlayerMovement::OnTriggerShift);
	input->BindAction(ia_movePressed, ETriggerEvent::Triggered, this, &UPlayerMovement::BeginDodge);
	input->BindAction(ia_doubleTap, ETriggerEvent::Completed, this, &UPlayerMovement::StartDodge);
}

void UPlayerMovement::OnChangedCameraMode(EPlayerCameraMode mode)
{
	Super::OnChangedCameraMode(mode);

	switch (mode)
	{
	case EPlayerCameraMode::Default:
		movementComp->bOrientRotationToMovement = true;
		if (movementComp->MaxWalkSpeed < sprintSpeed)
		{
			movementComp->MaxWalkSpeed = walkSpeed;
		}
		break;
	case EPlayerCameraMode::Anchored:
		movementComp->bOrientRotationToMovement = false;
		if (movementComp->MaxWalkSpeed < sprintSpeed)
		{
			movementComp->MaxWalkSpeed = walkSpeed;
		}
		break;
	case EPlayerCameraMode::Focus:
		movementComp->bOrientRotationToMovement = false;
		movementComp->MaxWalkSpeed = strollSpeed;
		break;
	}
}

void UPlayerMovement::Move(const FInputActionValue& actionValue)
{
	if (me->bIsBlockControl)
	{
		return;
	}
	
	FVector2D value = actionValue.Get<FVector2D>();
	me->AddMovementInput(FTransform(me->GetControlRotation()).TransformVector(FVector(value.X, value.Y, 0)));
}

void UPlayerMovement::Rotate(const FInputActionValue& actionValue)
{
	FVector2D value = actionValue.Get<FVector2D>();
	
	me->AddControllerYawInput(value.X);

	float pitch = me->GetControlRotation().GetNormalized().Pitch;

	if (me->GetPlayerCameraMode() != EPlayerCameraMode::Default)
	{
		
		if (value.Y > 0 && pitch < -45.f)
		{
			return;
		}

		if (value.Y < 0 && pitch > 45.f)
		{
			return;
		}
		
		anim->OnModifyWaistRotation(me->GetControlRotation());
	}
	
	me->AddControllerPitchInput(value.Y);
}

void UPlayerMovement::ActionJump(const FInputActionValue& actionValue)
{
	if (me->bIsBlockControl)
	{
		return;
	}
	
	me->Jump();
}

void UPlayerMovement::OnTriggerShift(const FInputActionValue& actionValue)
{
	if (me->bIsBlockControl)
	{
		return;
	}
	
	bool value = actionValue.Get<bool>();

	EPlayerCameraMode currentCameraMode = me->GetPlayerCameraMode();
	
	if (currentCameraMode == EPlayerCameraMode::Anchored)
	{
		// zoom mode
		movementComp->MaxWalkSpeed = walkSpeed;
		onEventCameraSlowMode.Execute(value);
	}
	else
	{
		float speed = value? sprintSpeed : walkSpeed;
	
		movementComp->MaxWalkSpeed = speed;
	}

	if (currentCameraMode == EPlayerCameraMode::Focus)
	{
		me->SetPlayerCameraMode(EPlayerCameraMode::Default);
	}
}

void UPlayerMovement::BeginDodge(const FInputActionValue& actionValue)
{
	if (me->bIsBlockControl)
	{
		return;
	}
	
	if (bIsPlayingDodge)
	{
		return;
	}
	
	dodgeValue = actionValue.Get<FVector2D>();
}

void UPlayerMovement::StartDodge()
{
	if (me->bIsBlockControl)
	{
		return;
	}
	
	if (bIsPlayingDodge)
	{
		return;
	}

	if (dodgeValue == FVector2D::ZeroVector)
	{
		return;
	}
	
	anim->OnStartDodge();
}

void UPlayerMovement::PlayingDodge()
{
	FVector dir = FVector(dodgeValue.X, dodgeValue.Y, 0);
	FVector dodgeDirection = FTransform(me->GetControlRotation()).TransformVector(dir).GetSafeNormal();
	
	FVector p0 = me->GetActorLocation();
	FVector vt = dodgeSpeed * GetWorld()->DeltaTimeSeconds * dodgeDirection;

	FVector forwardVector = me->GetActorForwardVector();

	float dotProduct = FVector::DotProduct(forwardVector, dodgeDirection);
	float radians = FMath::Acos(dotProduct);
	float degrees = FMath::RadiansToDegrees(radians);

	if (degrees > 1.f)
	{
		FRotator targetRotation = FRotationMatrix::MakeFromX(dodgeDirection).Rotator();
		FRotator r0 = me->GetControlRotation();

		FRotator newRotation = FMath::RInterpTo(r0, targetRotation, GetWorld()->DeltaTimeSeconds, 360.f);
		me->SetActorRotation(newRotation);
	}

	me->SetActorLocation(p0 + vt);
}

void UPlayerMovement::EndDodge()
{
	dodgeValue = FVector2D::ZeroVector;
	bIsPlayingDodge = false;
}
