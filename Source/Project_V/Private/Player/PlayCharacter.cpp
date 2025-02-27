// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/HUD.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCameraMode.h"
#include "Player/Weapon/PlayerWeapon.h"
#include "Player/Component/PlayerCameraSwitcher.h"
#include "Player/Component/PlayerCombat.h"
#include "Player/Component/PlayerFocusMode.h"
#include "Player/Component/PlayerMovement.h"
#include "UI/GameStateUI.h"
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

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmp_imc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'"));

	if (tmp_imc.Succeeded())
	{
		imc = tmp_imc.Object;
	}

	ConstructorHelpers::FClassFinder<APlayerWeapon> tmp_bow(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_PlayerBow.BP_PlayerBow_C'"));

	if (tmp_bow.Succeeded())
	{
		bowFactory = tmp_bow.Class;
	}

	ConstructorHelpers::FClassFinder<APlayerWeapon> tmp_tripcaster(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_Tripcaster.BP_Tripcaster_C'"));

	if (tmp_tripcaster.Succeeded())
	{
		tripcasterFactory = tmp_tripcaster.Class;
	}

	ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> tmp(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Player.ABP_Player_C'"));
	if (tmp.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(tmp.Object);
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

	if ((bow = GetWorld()->SpawnActor<APlayerWeapon>(bowFactory)))
	{
		//TODO:: 화살을 첨부터 활에 장착할지 말지 고민하고 결정. 지금은 임시
		bow->SpawnArrowInBow();
		bow->AttachSocket(GetMesh(), TEXT("BowSocket"), false);
	}

	if ((tripcaster = GetWorld()->SpawnActor<APlayerWeapon>(tripcasterFactory)))
	{
		//TODO:: 화살을 첨부터 활에 장착할지 말지 고민하고 결정. 지금은 임시
		tripcaster->SpawnArrowInBow();
		tripcaster->AttachSocket(GetMesh(), TEXT("CasterSocket"), false);
		tripcaster->SetVisibility(false);
		
		tripcaster->onCompleteFire.AddDynamic(this, &APlayCharacter::CheckPutWeaponTimer);
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
	AddEventHandler(focusMode, &UPlayerFocusMode::OnChangedCameraMode);

	cameraSwitcher->SetCameraSlowMode(false);

	combatComp->AddHandler(this, &APlayCharacter::IsNotAnchoredMode);
	combatComp->AddHandler(cameraSwitcher, &UPlayerCameraSwitcher::SetCameraSlowMode);

	movementComp->AddHandler(cameraSwitcher, &UPlayerCameraSwitcher::SetCameraSlowMode);
	movementComp->AddBaseEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::OnChangedCameraMode);

	focusMode->AddBaseEventHandler(cameraSwitcher, &UPlayerCameraSwitcher::OnChangedCameraMode);

	ChangeWeapon(bow);
	
}

void APlayCharacter::CheckPutWeaponTimer(bool bComplete)
{
	ClearPutWeaponTimer();
				
	if (bComplete)
	{
		StartTimerPutWeapon();
	}
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

void APlayCharacter::SpawnArrow()
{
	if (currentWeapon.IsValid())
	{
		currentWeapon->SpawnArrow(GetMesh(), currentWeapon->GetPickProjectileSocket());
	}
}

void APlayCharacter::PlaceArrowOnBow()
{
	if (currentWeapon.IsValid())
	{
		currentWeapon->PlaceArrowOnBow();
		combatComp->bIsCompleteReload = true;
	}
}

void APlayCharacter::PickWeapon()
{
	if (holdingWeapon.IsValid())
	{
		currentWeapon->AttachSocket(GetMesh(), currentWeapon->GetSlotSocket(), false);
		holdingWeapon = nullptr;
	}
	else
	{
		currentWeapon->AttachSocket(GetMesh(), currentWeapon->GetGripSocket(), true);
		holdingWeapon = currentWeapon;
	}
}

void APlayCharacter::ChangeWeapon(APlayerWeapon* weapon)
{
	if (currentWeapon == weapon)
	{
		return;
	}
	
	if (currentWeapon.IsValid())
	{
		currentWeapon->AttachSocket(GetMesh(), currentWeapon->GetSlotSocket(), false);
		currentWeapon->RevertProjectile();
		currentWeapon->SetVisibility(false);
	}

	anim->weaponType = weapon->GetWeaponType();
	weapon->SetVisibility(true);
	currentWeapon = weapon;
	
	anim->weaponChanged = true;
	if (holdingWeapon.IsValid())
	{
		holdingWeapon = nullptr;
		anim->OnPlayEquip();
	}
	
	combatComp->bIsCompleteReload = true;

	ui->ChangeEquippedWeaponUI(weapon->IsBase());

	if (currentCameraMode != EPlayerCameraMode::Anchored)
	{
		StartTimerPutWeapon();
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

void APlayCharacter::ClearPutWeaponTimer()
{
	if (timerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(timerHandle);
		timerHandle.Invalidate();
	}
}

void APlayCharacter::StartTimerPutWeapon()
{
	ClearPutWeaponTimer();
	
	TWeakObjectPtr<APlayCharacter> weakThis = this;
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
}

void APlayCharacter::SetPlayerCameraMode(EPlayerCameraMode mode)
{
	prevCameraMode = currentCameraMode;
	currentCameraMode = mode;
	
	onEventCameraModeChanged.Broadcast(mode);
	
	ui->SetVisibleUI(mode);
	

	switch (mode)
	{
	case EPlayerCameraMode::Default:
		bUseControllerRotationYaw = false;
		if (currentWeapon.IsValid())
		{
			currentWeapon->PlaceOrSpawnArrow();
		}
		StartTimerPutWeapon();
		break;
	case EPlayerCameraMode::Anchored:
		bUseControllerRotationYaw = true;
		if (timerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(timerHandle);
			timerHandle.Invalidate();
		}
		if (currentWeapon.IsValid())
		{
			currentWeapon->SpawnArrowInBow();
		}
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

void APlayCharacter::OnAnchoredMode()
{
	if (!holdingWeapon.IsValid())
	{
		anim->OnPlayEquip();
	}
	
	SetPlayerCameraMode(EPlayerCameraMode::Anchored);
}

void APlayCharacter::Fire(FVector velocity, float alpha)
{
	if (holdingWeapon.IsValid())
	{
		bool twice = holdingWeapon->Fire(velocity, alpha);
		
		anim->OnFire(twice);
	}
}

void APlayCharacter::HitDamage(float damage, FVector forward)
{
	float degrees;	

	TakeDamageInternal(damage, forward, degrees);
	
	anim->OnDamaged(degrees);
}

void APlayCharacter::HitLargeDamage(float damage, FVector forward)
{
	float degrees;	

	TakeDamageInternal(damage, forward, degrees);

	anim->OnLargeDamaged(degrees);
}

void APlayCharacter::TakeDamageInternal(float damage, FVector forward, float& degrees)
{
	if (currentHealth <= 0)
	{
		return;
	}
	
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
}

void APlayCharacter::GameOver()
{
	SetPlayerCameraMode(EPlayerCameraMode::Default);
	anim->OnDead();
	ShowGameStateUI.Broadcast(false);
	ClearPutWeaponTimer();

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
	bool hidden = !visible;
	SetActorHiddenInGame(hidden);

	if (currentWeapon.IsValid())
	{
		currentWeapon->SetActorHiddenInGame(hidden);
	}
}
