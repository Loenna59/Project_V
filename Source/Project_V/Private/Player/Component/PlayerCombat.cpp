// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/PlayerCombat.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayCharacter.h"
#include "Player/PlayerAnimInstance.h"
#include "Player/Weapon/PlayerRangedWeapon.h"
#include "UI/CrosshairUI.h"
#include "UI/PlayerUI.h"


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
	
	if ((bow = GetWorld()->SpawnActor<APlayerRangedWeapon>(bowFactory)))
	{
		//TODO:: 화살을 첨부터 활에 장착할지 말지 고민하고 결정. 지금은 임시
		bow->SpawnArrowInBow();
		bow->AttachSocket(me->GetMesh(), TEXT("BowSocket"), false);
	}

	if ((tripcaster = GetWorld()->SpawnActor<APlayerRangedWeapon>(tripcasterFactory)))
	{
		//TODO:: 화살을 첨부터 활에 장착할지 말지 고민하고 결정. 지금은 임시
		tripcaster->SpawnArrowInBow();
		tripcaster->AttachSocket(me->GetMesh(), TEXT("CasterSocket"), false);
		tripcaster->SetVisibility(false);
		
		tripcaster->onCompleteFire.AddDynamic(this, &UPlayerCombat::CheckPutWeaponTimer);
	}
	
	if (anim && bow)
	{
		anim->SetWeaponAnim(bow->GetAnimInstance());
	}

	ChangeWeapon(bow);
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
}

void UPlayerCombat::OnChangedCameraMode(EPlayerCameraMode mode)
{
	Super::OnChangedCameraMode(mode);

	switch (mode)
	{
	case EPlayerCameraMode::Default:
		SetDrawStrength(0);
		if (currentWeapon.IsValid())
		{
			currentWeapon->PlaceOrSpawnArrow();
		}
		StartTimerPutWeapon();
		break;
	case EPlayerCameraMode::Anchored:
		ClearPutWeaponTimer();
		if (currentWeapon.IsValid())
		{
			currentWeapon->SpawnArrowInBow();
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
	if (!holdingWeapon.IsValid())
	{
		anim->OnPlayEquip();
	}
	
	onEventCameraModeChanged.Execute(EPlayerCameraMode::Anchored);
}

void UPlayerCombat::OnAnchorRelease()
{
	me->SetPlayerCameraMode(EPlayerCameraMode::Default);
	bIsCompleteReload = true;
}

void UPlayerCombat::OnPressedFire(const FInputActionValue& actionValue)
{
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
	if (currentWeapon.IsValid())
	{
		currentWeapon->SpawnArrow(me->GetMesh(), currentWeapon->GetPickProjectileSocket());
	}
}

void UPlayerCombat::PlaceArrowOnBow()
{
	if (currentWeapon.IsValid())
	{
		currentWeapon->PlaceArrowOnBow();
		bIsCompleteReload = true;
	}
}

void UPlayerCombat::PickWeapon()
{
	if (holdingWeapon.IsValid())
	{
		currentWeapon->AttachSocket(me->GetMesh(), currentWeapon->GetSlotSocket(), false);
		holdingWeapon = nullptr;
	}
	else
	{
		currentWeapon->AttachSocket(me->GetMesh(), currentWeapon->GetGripSocket(), true);
		holdingWeapon = currentWeapon;
	}
}

void UPlayerCombat::ChangeWeapon(APlayerRangedWeapon* weapon)
{
	if (currentWeapon == weapon)
	{
		return;
	}
	
	if (currentWeapon.IsValid())
	{
		currentWeapon->AttachSocket(me->GetMesh(), currentWeapon->GetSlotSocket(), false);
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
	
	bIsCompleteReload = true;
	
	// me->ui->ChangeEquippedWeaponUI(weapon->IsBase());

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

			weakThis->anim->OnPlayEquip();
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
		bool twice = holdingWeapon->Fire(velocity, alpha);
		
		anim->OnFire(twice);
	}
}

void UPlayerCombat::SetVisibleEquippedWeapon(bool visible)
{
	if (currentWeapon.IsValid())
	{
		currentWeapon->SetVisibility(visible);
	}
}



