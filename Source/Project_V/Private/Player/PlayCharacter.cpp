// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/HUD.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCameraMode.h"
#include "Player/Component/PlayerCameraSwitcher.h"
#include "Player/Component/PlayerCombat.h"
#include "Player/Component/PlayerFocusMode.h"
#include "Player/Component/PlayerMovement.h"
#include "UI/PlayerHUD.h"
#include "UI/PlayerUI.h"

// Sets default values
APlayCharacter::APlayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	voiceComp = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceComp"));
	voiceComp->SetupAttachment(RootComponent);
	voiceComp->bAlwaysPlay = false;
	voiceComp->bAutoActivate = false;

	sheath = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sheath"));
	sheath->SetupAttachment(GetMesh(), TEXT("SheathSocket"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tmp_sheath(TEXT("/Script/Engine.StaticMesh'/Game/Assets/CyberpunkSamurai/Meshes/SM_Katana_Sheath.SM_Katana_Sheath'"));

	if (tmp_sheath.Succeeded())
	{
		sheath->SetStaticMesh(tmp_sheath.Object);
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmp_imc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'"));

	if (tmp_imc.Succeeded())
	{
		imc = tmp_imc.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> tmp(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Player.ABP_Player_C'"));
	if (tmp.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(tmp.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tmp_voice(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Player_Damaged.Player_Damaged'"));

	if (tmp_voice.Succeeded())
	{
		damagedVoice = tmp_voice.Object;
		voiceComp->SetSound(damagedVoice);
	}
	
	movementComp = CreateDefaultSubobject<UPlayerMovement>(TEXT("PlayerMovement"));
	combatComp = CreateDefaultSubobject<UPlayerCombat>(TEXT("PlayerCombat"));
	cameraSwitcher = CreateDefaultSubobject<UPlayerCameraSwitcher>(TEXT("CameraSwitcher"));
	focusMode = CreateDefaultSubobject<UPlayerFocusMode>(TEXT("FocusMode"));
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

	APlayerHUD* hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (hud)
	{
		ui = hud->GetPlayerUI();
		ui->SetVisibleUI(EPlayerCameraMode::Default);
		SetCurrentHealth(maxHealth);
	}

	anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	AddEventHandler(movementComp, &UPlayerMovement::OnChangedCameraMode);
	AddEventHandler(combatComp, &UPlayerCombat::OnChangedCameraMode);
	AddEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::OnChangedCameraMode);
	AddEventHandler(focusMode, &UPlayerFocusMode::OnChangedCameraMode);

	cameraSwitcher->SetCameraSlowMode(false);

	combatComp->AddHandler(this, &APlayCharacter::IsNotAnchoredMode);
	combatComp->AddHandler(cameraSwitcher, &UPlayerCameraSwitcher::SetCameraSlowMode);
	combatComp->AddBaseEventHandler(this, &APlayCharacter::SetPlayerCameraMode);

	movementComp->AddHandler(cameraSwitcher, &UPlayerCameraSwitcher::SetCameraSlowMode);
	movementComp->AddBaseEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::OnChangedCameraMode);

	focusMode->AddBaseEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::OnChangedCameraMode);
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
		focusMode->SetupInputBinding(pi);
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
	bUseControllerRotationYaw = mode != EPlayerCameraMode::Default;
}

bool APlayCharacter::IsNotAnchoredMode()
{
	return currentCameraMode != EPlayerCameraMode::Anchored;
}

void APlayCharacter::HitDamage(float damage, FVector forward)
{
	if (currentHealth <= 0)
	{
		return;
	}
	
	float degrees;	

	TakeDamageInternal(damage, forward, degrees);

	EDamageDirection dir = EDamageDirection::Front;

	if (degrees >= 270)
	{
		dir = EDamageDirection::Right;
	}
	else if (degrees >= 180)
	{
		dir = EDamageDirection::Back;
	}
	else if (degrees >= 90)
	{
		dir = EDamageDirection::Left;
	}
	
	anim->OnDamaged(dir);
}

void APlayCharacter::HitLargeDamage(float damage, FVector forward)
{
	if (currentHealth <= 0)
	{
		return;
	}
	
	float degrees;	

	TakeDamageInternal(damage, forward, degrees);

	anim->OnLargeDamaged(degrees);
}

void APlayCharacter::TakeDamageInternal(float damage, FVector forward, float& degrees)
{
	movementComp->EndDodge();
	
	currentHealth = FMath::Clamp(currentHealth - damage, 0, maxHealth);
	ui->SetHealthUI(currentHealth, maxHealth);
	if (currentHealth <= 0)
	{
		GameOver();
		return;
	}

	float dotProduct = FVector::DotProduct(GetActorForwardVector(), forward);
	float radians = FMath::Acos(dotProduct);
	degrees = FMath::RadiansToDegrees(radians);

	voiceComp->Play(0.05);
}

void APlayCharacter::GameOver()
{
	SetPlayerCameraMode(EPlayerCameraMode::Default);
	anim->OnDead();
	ShowGameStateUI.Broadcast(false);
	combatComp->ClearPutWeaponTimer();

	GetCharacterMovement()->Deactivate();
	
	APlayerController* pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSystem)
		{
			subSystem->ClearAllMappings();
		}
	}
}

void APlayCharacter::SetVisible(bool visible)
{
	GetMesh()->SetVisibility(visible);
	combatComp->SetVisibleEquippedWeapon(visible);
}
