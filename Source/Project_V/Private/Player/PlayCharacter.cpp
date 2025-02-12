// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MathUtil.h"
#include "Project_V.h"
#include "Components/SplineComponent.h"
#include "Player/PlayerAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APlayCharacter::APlayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmp_skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/CyberpunkSamurai/Meshes/SK_CyberpunkSamurai_WithHelmet.SK_CyberpunkSamurai_WithHelmet'"));

	if (tmp_skeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tmp_skeletalMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> tmp(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Player.ABP_Player_C'"));
	if (tmp.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(tmp.Object);
	}
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);

	springArmComp->SetRelativeLocation(FVector(0, 0, 80));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	springArmComp->bUsePawnControlRotation = true;
	springArmComp->bEnableCameraLag = true;
	springArmComp->bEnableCameraRotationLag = true;
	springArmComp->CameraLagSpeed = 5.f;
	springArmComp->CameraRotationLagSpeed = 5.f;

	anchoredSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("AnchoredSpringArmComp"));
	anchoredSpringArmComp->SetupAttachment(RootComponent);
	anchoredSpringArmComp->SetRelativeLocation(FVector(0, 0, 80));
	anchoredSpringArmComp->TargetArmLength = 100;

	anchoredCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("AnchoredCameraComp"));
	anchoredCameraComp->SetupAttachment(anchoredSpringArmComp);

	transitionSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("TransitionSpringArm"));
	transitionSpringArmComp->SetupAttachment(RootComponent);

	transitionCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TransitionCamera"));
	transitionCameraComp->SetupAttachment(transitionSpringArmComp);

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmp_imc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'"));

	if (tmp_imc.Succeeded())
	{
		imc = tmp_imc.Object;
	}

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
		ia_sprint = tmp_ia_sprint.Object;
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

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_anchored(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerAnchored.IA_PlayerAnchored'"));

	if (tmp_ia_anchored.Succeeded())
	{
		ia_anchored = tmp_ia_anchored.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_fire(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerFire.IA_PlayerFire'"));

	if (tmp_ia_fire.Succeeded())
	{
		ia_fire = tmp_ia_fire.Object;
	}

	weaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	weaponComp->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmp_bow(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Ranged/SKM_Bow.SKM_Bow'"));

	if (tmp_bow.Succeeded())
	{
		weaponComp->SetSkeletalMesh(tmp_bow.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tmp_weaponAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Bow.ABP_Bow_C'"));

	if (tmp_weaponAnim.Succeeded())
	{
		weaponComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		weaponComp->SetAnimInstanceClass(tmp_weaponAnim.Class);
	}
}

// Called when the game starts or when spawned
void APlayCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pc = CastChecked<APlayerController>(Controller);

	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSystem)
		{
			subSystem->AddMappingContext(imc, 0);
		}
	}

	anchoredCameraComp->SetActive(false);
	transitionCameraComp->SetActive(false);
}

// Called every frame
void APlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);

	direction = FVector::ZeroVector;
	bIsDodge = false;

	if (currentBlendCameraAlpha != targetBlendCameraAlpha)
	{
		float alpha = targetBlendCameraAlpha > 0?
			currentBlendCameraAlpha + DeltaTime * cameraTransitionSpeedMultiplier :
			currentBlendCameraAlpha - DeltaTime * cameraTransitionSpeedMultiplier;
		
		currentBlendCameraAlpha = FMath::Clamp(alpha, 0, 1);
		transitionSpringArmComp->TargetArmLength = FMath::Lerp(springArmComp->TargetArmLength, anchoredSpringArmComp->TargetArmLength, currentBlendCameraAlpha);
		transitionSpringArmComp->SetWorldLocation(FMath::Lerp(springArmComp->GetComponentLocation(), anchoredSpringArmComp->GetComponentLocation(), currentBlendCameraAlpha));
		transitionSpringArmComp->SocketOffset = FMath::Lerp(springArmComp->SocketOffset, anchoredSpringArmComp->SocketOffset, currentBlendCameraAlpha);

		if (currentBlendCameraAlpha == 0)
		{
			transitionCameraComp->SetActive(false);
			cameraComp->SetActive(true);
		}

		if (currentBlendCameraAlpha == 1)
		{
			transitionCameraComp->SetActive(false);
			anchoredCameraComp->SetActive(true);
		}
	}
}

// Called to bind functionality to input
void APlayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (pi)
	{
		pi->BindAction(ia_move, ETriggerEvent::Triggered, this, &APlayCharacter::Move);
		pi->BindAction(ia_rotate, ETriggerEvent::Triggered, this, &APlayCharacter::Rotate);
		pi->BindAction(ia_jump, ETriggerEvent::Started, this, &APlayCharacter::ActionJump);
		pi->BindAction(ia_sprint, ETriggerEvent::Triggered, this, &APlayCharacter::Sprint);
		pi->BindAction(ia_sprint, ETriggerEvent::Completed, this, &APlayCharacter::Sprint);
		pi->BindAction(ia_movePressed, ETriggerEvent::Triggered, this, &APlayCharacter::BeginDodge);
		pi->BindAction(ia_doubleTap, ETriggerEvent::Completed, this, &APlayCharacter::Dodge);
		pi->BindAction(ia_anchored, ETriggerEvent::Triggered, this, &APlayCharacter::OnAnchor);
		pi->BindAction(ia_anchored, ETriggerEvent::Completed, this, &APlayCharacter::OnAnchorRelease);
		pi->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayCharacter::OnPressedFire);
		pi->BindAction(ia_fire, ETriggerEvent::Completed, this, &APlayCharacter::OnReleasedFire);
	}
}

void APlayCharacter::Move(const FInputActionValue& actionValue)
{
	FVector2D value = actionValue.Get<FVector2D>();
	
	direction.X = value.X;
	direction.Y = value.Y;
}

void APlayCharacter::Rotate(const FInputActionValue& actionValue)
{
	FVector2D value = actionValue.Get<FVector2D>();

	
	AddControllerYawInput(value.X);
	AddControllerPitchInput(value.Y);

	// PrintLogFunc(TEXT("Pitch = %f, Yaw = %f"), GetControlRotation().GetNormalized().Pitch, GetControlRotation().GetNormalized().Yaw);
}

void APlayCharacter::ActionJump(const FInputActionValue& actionValue)
{
	ACharacter::Jump();
}

void APlayCharacter::Sprint(const FInputActionValue& actionValue)
{
	bool value = actionValue.Get<bool>();
	float speed = value? sprintSpeed : walkSpeed;

	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void APlayCharacter::BeginDodge(const FInputActionValue& actionValue)
{
	dodgeAxis = actionValue.Get<FVector2D>();

	// PrintLogFunc(TEXT("%f %f"), dodgeAxis.X, dodgeAxis.Y);
}

void APlayCharacter::Dodge()
{
	bIsDodge = dodgeAxis != FVector2D::ZeroVector;
}

void APlayCharacter::OnAnchor(const FInputActionValue& actionValue)
{
    cameraComp->SetActive(false);
	transitionCameraComp->SetActive(true);

	transitionSpringArmComp->bUsePawnControlRotation = anchoredSpringArmComp->bUsePawnControlRotation;
	transitionSpringArmComp->bEnableCameraLag = anchoredSpringArmComp->bEnableCameraLag;
	transitionSpringArmComp->bEnableCameraRotationLag = anchoredSpringArmComp->bEnableCameraRotationLag;
	transitionSpringArmComp->CameraLagSpeed = anchoredSpringArmComp->CameraLagSpeed;
	transitionSpringArmComp->CameraRotationLagSpeed = anchoredSpringArmComp->CameraRotationLagSpeed;
	
	targetBlendCameraAlpha = 1.f;

	bUseControllerRotationYaw = true;
	// bUseControllerRotationPitch = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	bIsAnchored = true;
}

void APlayCharacter::OnAnchorRelease(const FInputActionValue& actionValue)
{
	anchoredCameraComp->SetActive(false);
	transitionCameraComp->SetActive(true);

	transitionSpringArmComp->bUsePawnControlRotation = springArmComp->bUsePawnControlRotation;
	transitionSpringArmComp->bEnableCameraLag = springArmComp->bEnableCameraLag;
	transitionSpringArmComp->bEnableCameraRotationLag = springArmComp->bEnableCameraRotationLag;
	transitionSpringArmComp->CameraLagSpeed = springArmComp->CameraLagSpeed;
	transitionSpringArmComp->CameraRotationLagSpeed = springArmComp->CameraRotationLagSpeed;

	targetBlendCameraAlpha = 0.f;

	bUseControllerRotationYaw = false;
	// bUseControllerRotationPitch = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	drawStrength = 0;
	bIsAnchored = false;
}

void APlayCharacter::OnPressedFire(const FInputActionValue& actionValue)
{
	if (bIsAnchored)
	{
		drawStrength = FMathf::Min(drawStrength + GetWorld()->DeltaTimeSeconds * drawSpeedMultiplier, 100);
	}
	else
	{
		
	}
}

void APlayCharacter::OnReleasedFire(const FInputActionValue& actionValue)
{
	if (bIsAnchored)
	{
		drawStrength = 0;
	}
}
