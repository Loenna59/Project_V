// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/HUD.h"
#include "Player/FocusDome.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCameraMode.h"
#include "Player/PlayerWeapon.h"
#include "Player/Component/PlayerCameraSwitcher.h"
#include "Player/Component/PlayerCombat.h"
#include "Player/Component/PlayerMovement.h"
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

	movementComp = CreateDefaultSubobject<UPlayerMovement>(TEXT("PlayerMovement"));
	combatComp = CreateDefaultSubobject<UPlayerCombat>(TEXT("PlayerCombat"));
	cameraSwitcher = CreateDefaultSubobject<UPlayerCameraSwitcher>(TEXT("CameraSwitcher"));
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
		combatComp->bIsCompleteReload = true;
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

	anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (anim && bow)
	{
		anim->SetWeaponAnim(bow->GetAnimInstance());
	}

	AddEventHandler(movementComp, &UPlayerMovement::OnChangedCameraMode);
	AddEventHandler(combatComp, &UPlayerCombat::OnChangedCameraMode);
	AddEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::OnChangedCameraMode);

	cameraSwitcher->SetCameraSlowMode(false);

	combatComp->AddHandler(this, &APlayCharacter::IsNotAnchoredMode);
	combatComp->AddHandler(cameraSwitcher, &UPlayerCameraSwitcher::SetCameraSlowMode);

	movementComp->AddEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::SetCameraSlowMode);
	movementComp->AddEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::OnChangedCameraMode);
}

// Called every frame
void APlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	bIsShot = false;
}

// Called to bind functionality to input
void APlayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (pi)
	{
		movementComp->SetupInputBinding(pi);
		combatComp->SetupInputBinding(pi);
		pi->BindAction(ia_focus, ETriggerEvent::Triggered, this, &APlayCharacter::OnFocusOrScan);
		pi->BindAction(ia_focus, ETriggerEvent::Completed, this, &APlayCharacter::EndFocusOrScan);
	}
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
		cameraSwitcher->OnChangedCameraMode(EPlayerCameraMode::Default);
		return;
	}

	if (focusPressingTime > focusModeThreshold)
	{
		focusDome->Activate();
		cameraSwitcher->OnChangedCameraMode(EPlayerCameraMode::Focus);
	}
	
	focusPressingTime += GetWorld()->DeltaTimeSeconds;
}

void APlayCharacter::EndFocusOrScan()
{
	if (currentCameraMode != EPlayerCameraMode::Focus && focusPressingTime < focusModeThreshold)
	{
		//PrintLogFunc(TEXT("scan"));
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
		combatComp->bIsCompleteReload = true;
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

void APlayCharacter::SetPlayingDodge(bool isPlaying)
{
	movementComp->bIsPlayingDodge = isPlaying;

	if (isPlaying)
	{
		SetPlayerCameraMode(EPlayerCameraMode::Default);
	}
	else
	{
		// 강제로 리로드 처리
		combatComp->bIsCompleteReload = true;
		movementComp->EndDodge();
		
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
	
	onEventCameraModeChanged.Broadcast(mode);
	
	ui->SetVisibleUI(mode);
	
	TWeakObjectPtr<APlayCharacter> weakThis = this;

	switch (mode)
	{
	case EPlayerCameraMode::Default:
		bUseControllerRotationYaw = false;
		bow->PlaceOrSpawnArrow();
		focusDome->Deactivate();
		
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

				weakThis->anim->OnPlayEquip();
			},
			idleTimerDuration,
			false
		);
		break;
	case EPlayerCameraMode::Anchored:
		bUseControllerRotationYaw = true;
		if (timerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(timerHandle);
			timerHandle.Invalidate();
		}
		bow->SpawnArrowInBow();
		focusDome->Deactivate();
		break;
	case EPlayerCameraMode::Focus:
		bUseControllerRotationYaw = true;
		break;
	}
	
}

bool APlayCharacter::IsNotAnchoredMode()
{
	return currentCameraMode != EPlayerCameraMode::Anchored;
}
