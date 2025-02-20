// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Project_V.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FocusDome.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCameraMode.h"
#include "Player/PlayerWeapon.h"
#include "UI/CrosshairUI.h"
#include "UI/PlayerHUD.h"
#include "UI/PlayerUI.h"

// Sets default values
APlayCharacter::APlayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	GetCapsuleComponent()->SetCollisionProfileName("Player");

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmp_skeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/CyberpunkSamurai/Meshes/SK_CyberpunkSamurai_WithHelmet.SK_CyberpunkSamurai_WithHelmet'"));

	if (tmp_skeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tmp_skeletalMesh.Object);
	}
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);

	springArmComp->SetRelativeLocation(FVector(0, 0, 80));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	springArmComp->bUsePawnControlRotation = true;
	// springArmComp->bEnableCameraLag = true;
	//springArmComp->bEnableCameraRotationLag = true;
	springArmComp->CameraLagSpeed = 8.f;
	springArmComp->CameraRotationLagSpeed = 8.f;

	anchoredSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("AnchoredSpringArmComp"));
	
	anchoredSpringArmComp->SetupAttachment(RootComponent);
	anchoredSpringArmComp->SetRelativeLocation(FVector(0, 0, 80));
	anchoredSpringArmComp->TargetArmLength = 80;
	anchoredSpringArmComp->SocketOffset = FVector(0, 50, 0);
	anchoredSpringArmComp->bUsePawnControlRotation = false;
	anchoredSpringArmComp->ProbeSize = 12;

	anchoredCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("AnchoredCameraComp"));
	anchoredCameraComp->SetupAttachment(anchoredSpringArmComp);
	anchoredCameraComp->bUsePawnControlRotation = true;

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

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_focus(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerFocusMode.IA_PlayerFocusMode'"));

	if (tmp_ia_focus.Succeeded())
	{
		ia_focus = tmp_ia_focus.Object;
	}

	ConstructorHelpers::FClassFinder<APlayerWeapon> tmp_weapon(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_PlayerBow.BP_PlayerBow_C'"));

	if (tmp_weapon.Succeeded())
	{
		bowFactory = tmp_weapon.Class;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmp_montage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Player/Animation/Player_Bow_Hide_Montage.Player_Bow_Hide_Montage'"));

	if (tmp_montage.Succeeded())
	{
		equipWeaponMontage = tmp_montage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> tmp(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Player.ABP_Player_C'"));
	if (tmp.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(tmp.Object);
	}

	ConstructorHelpers::FClassFinder<AFocusDome> tmp_dome(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_FocusDome.BP_FocusDome_C'"));

	if (tmp_dome.Succeeded())
	{
		domeFactory = tmp_dome.Class;
	}

	targetFOV = maxFOV;
	targetMultiplier = releaseMotionMultiplier;
}

// Called when the game starts or when spawned
void APlayCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pc = Cast<APlayerController>(Controller);

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

	if ((bow = GetWorld()->SpawnActor<APlayerWeapon>(bowFactory)))
	{
		//TODO:: 화살을 첨부터 활에 장착할지 말지 고민하고 결정. 지금은 임시
		bow->SpawnArrowInBow();
		bIsCompleteReload = true;
		//

		bow->AttachSocket(GetMesh(), TEXT("BowSocket"), false);
	}

	if ((focusDome = GetWorld()->SpawnActor<AFocusDome>(domeFactory)))
	{
		focusDome->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		focusDome->SetActorRelativeLocation(FVector(0, 0, -200));
	}

	APlayerHUD* hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (hud)
	{
		ui = hud->GetPlayerUI();
		ui->SetVisibleUI(EPlayerCameraMode::Default);
		SetCurrentHealth(maxHealth);
	}

	UAnimInstance* anim = GetMesh()->GetAnimInstance();

	if (anim && bow)
	{
		if (UPlayerAnimInstance* playerAnim = Cast<UPlayerAnimInstance>(anim))
		{
			playerAnim->SetWeaponAnim(bow->GetAnimInstance());
		}
	}
}

// Called every frame
void APlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	bIsDodge = false;
	bIsShot = false;

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

	if (anchoredCameraComp->FieldOfView != targetFOV)
	{
		float fov = FMath::Lerp(anchoredCameraComp->FieldOfView, targetFOV, DeltaTime * targetMultiplier);
		anchoredCameraComp->SetFieldOfView(FMath::Clamp(fov, minFOV, maxFOV));
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
		pi->BindAction(ia_shift, ETriggerEvent::Triggered, this, &APlayCharacter::OnTriggerShift);
		pi->BindAction(ia_shift, ETriggerEvent::Completed, this, &APlayCharacter::OnTriggerShift);
		pi->BindAction(ia_movePressed, ETriggerEvent::Triggered, this, &APlayCharacter::BeginDodge);
		pi->BindAction(ia_doubleTap, ETriggerEvent::Completed, this, &APlayCharacter::Dodge);
		pi->BindAction(ia_anchored, ETriggerEvent::Started, this, &APlayCharacter::OnAnchor);
		pi->BindAction(ia_anchored, ETriggerEvent::Completed, this, &APlayCharacter::OnAnchorRelease);
		pi->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayCharacter::OnPressedFire);
		pi->BindAction(ia_fire, ETriggerEvent::Completed, this, &APlayCharacter::OnReleasedFire);
		pi->BindAction(ia_focus, ETriggerEvent::Triggered, this, &APlayCharacter::OnFocusOrScan);
		pi->BindAction(ia_focus, ETriggerEvent::Completed, this, &APlayCharacter::EndFocusOrScan);
	}
}

void APlayCharacter::Move(const FInputActionValue& actionValue)
{
	FVector2D value = actionValue.Get<FVector2D>();
	AddMovementInput(FTransform(GetControlRotation()).TransformVector(FVector(value.X, value.Y, 0)));
}

void APlayCharacter::Rotate(const FInputActionValue& actionValue)
{
	FVector2D value = actionValue.Get<FVector2D>();
	
	AddControllerYawInput(value.X);

	float pitch = GetControlRotation().GetNormalized().Pitch;

	if (currentCameraMode != EPlayerCameraMode::Default)
	{
		// PrintLogFunc(TEXT("Pitch = %f, Yaw = %f"), GetControlRotation().GetNormalized().Pitch, GetControlRotation().GetNormalized().Yaw);

		if (value.Y > 0 && pitch < -45.f)
		{
			return;
		}

		if (value.Y < 0 && pitch > 45.f)
		{
			return;
		}
	}
	
	AddControllerPitchInput(value.Y);
}

void APlayCharacter::ActionJump(const FInputActionValue& actionValue)
{
	ACharacter::Jump();
}

void APlayCharacter::OnTriggerShift(const FInputActionValue& actionValue)
{
	bool value = actionValue.Get<bool>();
	
	if (currentCameraMode == EPlayerCameraMode::Anchored)
	{
		// zoom mode
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		if (value)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), slowDilation); 
			targetFOV = minFOV;
			targetMultiplier = slowMotionMultiplier;
		}
		else
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f); 
			targetFOV = maxFOV;
			targetMultiplier = releaseMotionMultiplier;
		}
	}
	else
	{
		float speed = value? sprintSpeed : walkSpeed;
	
		GetCharacterMovement()->MaxWalkSpeed = speed;
	}

	if (currentCameraMode == EPlayerCameraMode::Focus)
	{
		SetPlayerCameraMode(EPlayerCameraMode::Default);
	}
}

void APlayCharacter::BeginDodge(const FInputActionValue& actionValue)
{
	if (bIsPlayingDodge)
	{
		return;
	}

	prevDodgeAxis = dodgeAxis;
	dodgeAxis = actionValue.Get<FVector2D>();

	// PrintLogFunc(TEXT("%f %f"), dodgeAxis.X, dodgeAxis.Y);
}

void APlayCharacter::Dodge()
{
	if (bIsPlayingDodge)
	{
		return;
	}

	if (dodgeAxis.X != 0 && FMath::IsWithin(prevDodgeAxis.X + dodgeAxis.X, -1.5f, 1.5f))
	{
		prevDodgeAxis = FVector2D::ZeroVector;
		dodgeAxis = FVector2D::ZeroVector;
		return;
	}

	 
	if (dodgeAxis.Y != 0 && FMath::IsWithin(prevDodgeAxis.Y + dodgeAxis.Y, -1.5f, 1.5f))
	{
		prevDodgeAxis = FVector2D::ZeroVector;
		dodgeAxis = FVector2D::ZeroVector;
		return;
	}
	
	bIsDodge = dodgeAxis != FVector2D::ZeroVector;
	ChangeToDefaultCamera();
}

void APlayCharacter::ChangeToAnchoredCamera()
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
}

void APlayCharacter::OnAnchor()
{
	if (!holdingWeapon.IsValid() && equipWeaponMontage)
	{
		PlayAnimMontage(equipWeaponMontage);
	}
	
	SetPlayerCameraMode(EPlayerCameraMode::Anchored);
}

void APlayCharacter::ChangeToDefaultCamera()
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

	anchoredCameraComp->SetFieldOfView(maxFOV);
}

void APlayCharacter::OnAnchorRelease()
{
	SetPlayerCameraMode(EPlayerCameraMode::Default);
	bIsCompleteReload = true;
}

void APlayCharacter::OnPressedFire(const FInputActionValue& actionValue)
{
	if (currentCameraMode != EPlayerCameraMode::Anchored)
	{
		return;
	}
	
	if (!bIsCompleteReload)
	{
		return;
	}
	
	elapsedDrawingTime += GetWorld()->DeltaTimeSeconds;

	float t = FMath::Clamp(elapsedDrawingTime / drawDuration, 0, 1);

	//EaseOutQuad = 1 - (1 - t)^2
	float quadT = 1 - FMath::Pow(1 - t, 2);
	
	// 시간에 따라 증가
	SetDrawStrength(FMath::Lerp(0, targetDrawStrength, quadT));
}

void APlayCharacter::OnReleasedFire(const FInputActionValue& actionValue)
{
	if (currentCameraMode != EPlayerCameraMode::Anchored)
	{
		return;
	}

	if (drawStrength < drawingThreshold)
	{
		return;
	}

	bow->Fire(CalculateAnimToVector(), drawStrength / targetDrawStrength);

	bIsCompleteReload = false;
	bIsShot = true;
	SetDrawStrength(0);
}

void APlayCharacter::OnFocusOrScan(const FInputActionValue& actionValue)
{
	if (!focusDome)
	{
		return;
	}
	
	if (currentCameraMode == EPlayerCameraMode::Focus)
	{
		focusDome->Deactivate();
		ChangeToDefaultCamera();
		return;
	}

	if (focusPressingTime > focusModeThreshold)
	{
		focusDome->Activate();
		ChangeToAnchoredCamera();
	}
	
	focusPressingTime += GetWorld()->DeltaTimeSeconds;
	
	//PrintLogFunc(TEXT("%f"), actionValue.Get<float>());
}

void APlayCharacter::EndFocusOrScan()
{
	if (currentCameraMode != EPlayerCameraMode::Focus && focusPressingTime < focusModeThreshold)
	{
		PrintLogFunc(TEXT("scan"));
		focusPressingTime = 0;
		return;
	}
	
	focusPressingTime = 0;
	SetPlayerCameraMode(currentCameraMode == EPlayerCameraMode::Focus? EPlayerCameraMode::Default : EPlayerCameraMode::Focus);
}

void APlayCharacter::SpawnArrow()
{
	if (bow)
	{
		bow->SpawnArrow(GetMesh(), TEXT("PickArrowSocket"));
	}
}

void APlayCharacter::PlaceArrowOnBow()
{
	if (bow)
	{
		bow->PlaceArrowOnBow();
		bIsCompleteReload = true;
	}
}

void APlayCharacter::PickWeapon()
{
	if (holdingWeapon.IsValid())
	{
		bow->AttachSocket(GetMesh(), TEXT("BowSocket"), false);
		holdingWeapon = nullptr;
	}
	else
	{
		bow->AttachSocket(GetMesh(), TEXT("hand_lSocket"), true);
		holdingWeapon = bow;
	}
}

FVector APlayCharacter::CalculateAnimToVector()
{
	FVector start = anchoredCameraComp->GetComponentLocation();
	FVector end = start + anchoredCameraComp->GetForwardVector() * 10000;

	TArray<AActor*> ignores;
	ignores.Add(this);

	FHitResult hitResult;

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);

	return isHit? hitResult.Location : hitResult.TraceEnd;
}

void APlayCharacter::SetDrawStrength(float strength)
{
	drawStrength = strength;

	if (ui)
	{
		ui->Crosshair->UpdateCircle(strength);
	}

	if (drawStrength < 0.01f)
	{
		targetFOV = maxFOV;
		targetMultiplier = releaseMotionMultiplier;
		elapsedDrawingTime = 0;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f); 
	}
}

void APlayCharacter::SetPlayingDodge(bool isPlaying)
{
	bIsPlayingDodge = isPlaying;


	if (isPlaying)
	{
		SetPlayerCameraMode(EPlayerCameraMode::Default);
	}
	else
	{
		// 강제로 리로드 처리
		bIsCompleteReload = true;
		prevDodgeAxis = FVector2D::ZeroVector;
		dodgeAxis = FVector2D::ZeroVector;

		if (prevCameraMode == EPlayerCameraMode::Focus)
		{
			return;
		}
		SetPlayerCameraMode(prevCameraMode);
	}
}

void APlayCharacter::SetCurrentHealth(float health)
{
	currentHealth = health;
	if (ui)
	{
		ui->SetHealthUI(currentHealth, maxHealth);
	}
}

void APlayCharacter::SetPlayerCameraMode(EPlayerCameraMode mode)
{
	prevCameraMode = currentCameraMode;
	currentCameraMode = mode;

	ui->SetVisibleUI(currentCameraMode);
	
	TWeakObjectPtr<APlayCharacter> weakThis = this;
	
	switch (currentCameraMode)
	{
	case EPlayerCameraMode::Default:
		ChangeToDefaultCamera();
		SetDrawStrength(0);
		bow->PlaceOrSpawnArrow();
		focusDome->Deactivate();
		if (GetCharacterMovement()->MaxWalkSpeed < sprintSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		}
		
		GetWorldTimerManager().SetTimer(
			timerHandle,
			[weakThis] ()
			{
				if (!weakThis.IsValid())
				{
					return;
				}
				
				if (!weakThis->holdingWeapon.IsValid())
				{
					return;
				}

				weakThis->PlayAnimMontage(weakThis->equipWeaponMontage);
			},
			idleTimerDuration,
			false
		);
		break;
	case EPlayerCameraMode::Anchored:
		if (timerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(timerHandle);
			timerHandle.Invalidate();
		}
		ChangeToAnchoredCamera();
		bow->SpawnArrowInBow();
		focusDome->Deactivate();
		if (GetCharacterMovement()->MaxWalkSpeed < sprintSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		}
		break;
	case EPlayerCameraMode::Focus:
		ChangeToAnchoredCamera();
		GetCharacterMovement()->MaxWalkSpeed = strollSpeed;
		break;
	}
	
}
