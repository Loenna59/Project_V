// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/PlayerCombat.h"

#include "EnhancedInputComponent.h"
#include "Project_V.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayCharacter.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/Weapon/Katana.h"
#include "Player/Weapon/PlayerRangedWeapon.h"
#include "UI/CrosshairUI.h"
#include "UI/PlayerUI.h"
#include "Player/Weapon/PlayerWeapon.h"



// Sets default values for this component's properties
UPlayerCombat::UPlayerCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_quick(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerQuick.IA_PlayerQuick'"));

	if (tmp_ia_quick.Succeeded())
	{
		ia_quick = tmp_ia_quick.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_wheel(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerWheel.IA_PlayerWheel'"));

	if (tmp_ia_wheel.Succeeded())
	{
		ia_wheel = tmp_ia_wheel.Object;
	}

	ConstructorHelpers::FClassFinder<AKatana> tmp_katana(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_Katana.BP_Katana_C'"));

	if (tmp_katana.Succeeded())
	{
		katanaFactory = tmp_katana.Class;
	}

	ConstructorHelpers::FClassFinder<APlayerRangedWeapon> tmp_bow(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_PlayerBow.BP_PlayerBow_C'"));

	if (tmp_bow.Succeeded())
	{
		bowFactory = tmp_bow.Class;
	}

	ConstructorHelpers::FClassFinder<APlayerRangedWeapon> tmp_tripcaster(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_Tripcaster.BP_Tripcaster_C'"));

	if (tmp_tripcaster.Succeeded())
	{
		tripcasterFactory = tmp_tripcaster.Class;
	}
}


// Called when the game starts
void UPlayerCombat::BeginPlay()
{
	Super::BeginPlay();
	
	cameraComp = me->anchoredCameraComp;

	if ((katana = GetWorld()->SpawnActor<AKatana>(katanaFactory)))
	{
		katana->AttachSocket(me->GetMesh(), TEXT("SheathSocket"), false);
	}
	
	if ((bow = GetWorld()->SpawnActor<APlayerRangedWeapon>(bowFactory)))
	{
		bow->SpawnArrowInBow();
		bow->AttachSocket(me->GetMesh(), TEXT("BowSocket"), false);
	}

	if ((tripcaster = GetWorld()->SpawnActor<APlayerRangedWeapon>(tripcasterFactory)))
	{
		tripcaster->SpawnArrowInBow();
		tripcaster->AttachSocket(me->GetMesh(), TEXT("CasterSocket"), false);
		tripcaster->SetVisibility(false);
		
		tripcaster->onCompleteFire.AddDynamic(this, &UPlayerCombat::CheckPutWeaponTimer);
	}
	
	if (anim && bow)
	{
		anim->SetWeaponAnim(bow->GetAnimInstance());
	}

	currentRangedWeapon = bow;
	anim->weaponType = bow->GetWeaponType();
	bIsCompleteReload = true;
	anim->weaponChanged = true;
}

FVector UPlayerCombat::CalculateAnimToVector()
{
	FVector start = cameraComp->GetComponentLocation();
	FVector end = start + cameraComp->GetForwardVector() * 10000;

	TArray<AActor*> ignores;
	ignores.Add(me);

	FHitResult hitResult;

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);

	return isHit? hitResult.Location : hitResult.TraceEnd;
}

void UPlayerCombat::SetupInputBinding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBinding(input);

	input->BindAction(ia_anchored, ETriggerEvent::Started, this, &UPlayerCombat::OnAnchor);
	input->BindAction(ia_anchored, ETriggerEvent::Completed, this, &UPlayerCombat::OnAnchorRelease);
	input->BindAction(ia_fire, ETriggerEvent::Triggered, this, &UPlayerCombat::OnPressedFire);
	input->BindAction(ia_fire, ETriggerEvent::Completed, this, &UPlayerCombat::OnReleasedFire);
	input->BindAction(ia_fire, ETriggerEvent::Started, this, &UPlayerCombat::OnMeleeAttack);
	input->BindAction(ia_quick, ETriggerEvent::Triggered, this, &UPlayerCombat::OnSwapWeapon);
	input->BindAction(ia_wheel, ETriggerEvent::Triggered, this, &UPlayerCombat::OnWheelWeapon);
}

void UPlayerCombat::OnChangedCameraMode(EPlayerCameraMode mode)
{
	Super::OnChangedCameraMode(mode);

	switch (mode)
	{
	case EPlayerCameraMode::Default:
		SetDrawStrength(0);
		if (currentRangedWeapon.IsValid())
		{
			currentRangedWeapon->PlaceOrSpawnArrow();
		}
		StartTimerPutWeapon();
		break;
	case EPlayerCameraMode::Anchored:
		ClearPutWeaponTimer();
		if (currentRangedWeapon.IsValid())
		{
			currentRangedWeapon->SpawnArrowInBow();
		}
		break;
	case EPlayerCameraMode::Focus:
		// 강제로 리로드 처리
		bIsCompleteReload = true;
		break;
	}
}

void UPlayerCombat::OnAnchor()
{
	if (katanaPlayState == KatanaPlayState::Acting)
	{
		return;
	}
	
	onEventCameraModeChanged.Execute(EPlayerCameraMode::Anchored);
	
	if (holdingWeapon.IsValid())
	{
		if (katanaPlayState != KatanaPlayState::Unequipped)
		{
			holdingWeapon = nullptr;
			katanaPlayState = KatanaPlayState::Unequipped;
			katana->AttachSocket(me->GetMesh(), katana->GetSlotSocket(), false);
			anim->isHoldingKatana = false;
			OnEndKatanaCombo();
		}
		else
		{
			return;
		}
	}

	anim->weaponType = currentRangedWeapon->GetWeaponType();
	anim->OnPlayEquip();
}

void UPlayerCombat::OnAnchorRelease()
{
	me->SetPlayerCameraMode(EPlayerCameraMode::Default);
	bIsCompleteReload = true;
}

void UPlayerCombat::OnPressedFire(const FInputActionValue& actionValue)
{
	if (katanaPlayState != KatanaPlayState::Unequipped)
	{
		return;
	}
	
	if (onEventCheckCameraMode.Execute())
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

void UPlayerCombat::OnReleasedFire(const FInputActionValue& actionValue)
{
	if (onEventCheckCameraMode.Execute())
	{
		return;
	}

	if (drawStrength < drawingThreshold)
	{
		return;
	}

	Fire(CalculateAnimToVector(), drawStrength / targetDrawStrength);

	bIsCompleteReload = false;
	SetDrawStrength(0);
}

void UPlayerCombat::OnSwapWeapon(const FInputActionValue& actionValue)
{
	float value = actionValue.Get<float>();
	int32 intValue = FMath::Floor(value);

	switch (intValue)
	{
	case 1:
		ChangeWeapon(bow);
		break;
	case 2:
		ChangeWeapon(tripcaster);
		break;
	default:
		break;
	}
}

void UPlayerCombat::OnWheelWeapon(const FInputActionValue& actionValue)
{
	if (currentRangedWeapon == bow)
	{
		ChangeWeapon(tripcaster);
	}
	else
	{
		ChangeWeapon(bow);
	}
}

void UPlayerCombat::OnMeleeAttack()
{
	if (me->bIsBlockControl)
	{
		return;
	}
	
	if (katanaPlayState == KatanaPlayState::Holding)
	{
		anim->OnComboKatana(comboIndex);
		comboIndex = (comboIndex + 1) % 4;
		return;
	}
    
    if (katanaPlayState == KatanaPlayState::Unequipped && onEventCheckCameraMode.Execute())
    {
    	currentRangedWeapon->AttachSocket(me->GetMesh(), currentRangedWeapon->GetSlotSocket(), false);
		katanaPlayState = KatanaPlayState::Acting;
    	holdingWeapon = nullptr;
    	anim->OnStartKatana();
    	StartTimerPutWeapon();
    }
}

void UPlayerCombat::SetDrawStrength(float strength)
{
	drawStrength = strength;

	anim->OnSetDrawStrength(strength);

	if (me->ui)
	{
		me->ui->Crosshair->UpdateCircle(strength);
	}

	if (drawStrength < 0.01f)
	{
		onEventReleased.Broadcast(false);
		elapsedDrawingTime = 0;
	}
}

void UPlayerCombat::SpawnArrow()
{
	if (currentRangedWeapon.IsValid())
	{
		currentRangedWeapon->SpawnArrow(me->GetMesh(), currentRangedWeapon->GetPickProjectileSocket());
	}
}

void UPlayerCombat::PlaceArrowOnBow()
{
	if (currentRangedWeapon.IsValid())
	{
		currentRangedWeapon->PlaceArrowOnBow();
		bIsCompleteReload = true;
	}
}

void UPlayerCombat::PickWeapon()
{
	if (katanaPlayState != KatanaPlayState::Unequipped)
	{
		if (holdingWeapon.IsValid())
		{
			katana->AttachSocket(me->GetMesh(), katana->GetSlotSocket(), false);
			holdingWeapon = nullptr;
			katanaPlayState = KatanaPlayState::Unequipped;
			return;
		}
		
		katana->AttachSocket(me->GetMesh(), katana->GetGripSocket(), false);
		holdingWeapon = katana;
		return;
	}
	
	if (holdingWeapon.IsValid())
	{
		currentRangedWeapon->AttachSocket(me->GetMesh(), currentRangedWeapon->GetSlotSocket(), false);
		holdingWeapon = nullptr;
	}
	else
	{
		currentRangedWeapon->AttachSocket(me->GetMesh(), currentRangedWeapon->GetGripSocket(), true);
		holdingWeapon = currentRangedWeapon;
	}
}

void UPlayerCombat::ChangeWeapon(APlayerRangedWeapon* weapon)
{
	if (currentRangedWeapon == weapon)
	{
		return;
	}
	
	if (currentRangedWeapon.IsValid())
	{
		currentRangedWeapon->AttachSocket(me->GetMesh(), currentRangedWeapon->GetSlotSocket(), false);
		currentRangedWeapon->RevertProjectile();
		currentRangedWeapon->SetVisibility(false);
	}

	anim->weaponType = weapon->GetWeaponType();
	weapon->SetVisibility(true);
	currentRangedWeapon = weapon;
	
	anim->weaponChanged = true;
	if (holdingWeapon.IsValid())
	{
		holdingWeapon = nullptr;
		anim->OnPlayEquip();
	}
	
	bIsCompleteReload = true;
	
	me->ui->ChangeEquippedWeaponUI(weapon->GetWeaponType() == EWeaponType::Base);

	if (me->GetPlayerCameraMode() != EPlayerCameraMode::Anchored)
	{
		StartTimerPutWeapon();
	}
}

void UPlayerCombat::ClearPutWeaponTimer()
{
	if (timerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		timerHandle.Invalidate();
	}
}

void UPlayerCombat::StartTimerPutWeapon()
{
	ClearPutWeaponTimer();
	
	TWeakObjectPtr<UPlayerCombat> weakThis = this;
	GetWorld()->GetTimerManager().SetTimer(
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

			if (weakThis->katanaPlayState == KatanaPlayState::Unequipped)
			{
				weakThis->anim->OnPlayEquip();
			}
			else
			{
				weakThis->anim->OnHideKatana();
			}
		},
		idleTimerDuration,
		false
	);
}

void UPlayerCombat::CheckPutWeaponTimer(bool bComplete)
{
	ClearPutWeaponTimer();
				
	if (bComplete)
	{
		StartTimerPutWeapon();
	}
}

void UPlayerCombat::Fire(FVector velocity, float alpha)
{
	if (holdingWeapon.IsValid())
	{
		bool twice = currentRangedWeapon->Fire(velocity, alpha);
		
		anim->OnFire(twice);
	}
}

void UPlayerCombat::SetVisibleEquippedWeapon(bool visible)
{
	if (currentRangedWeapon.IsValid())
	{
		currentRangedWeapon->SetVisibility(visible);
	}
}

void UPlayerCombat::SetVisibleKatana(bool visible)
{
	if (katana->IsValidLowLevel())
	{
		katana->SetVisibility(visible);
	}
}

void UPlayerCombat::OnStartTraceKatanaChannel()
{
	katanaPlayState = KatanaPlayState::Acting;
	if (katana->IsValidLowLevel())
	{
		katana->StartTraceChannel();
	}
}

void UPlayerCombat::OnEndTraceKatanaChannel()
{
	katanaPlayState = KatanaPlayState::Holding;
	if (katana->IsValidLowLevel())
	{
		katana->EndTraceChannel();
	}
	StartTimerPutWeapon();
}

void UPlayerCombat::OnEndKatanaAnimState()
{
	anim->isHoldingKatana = false;
}

void UPlayerCombat::OnEndKatanaCombo()
{
	comboIndex = 0;
}




