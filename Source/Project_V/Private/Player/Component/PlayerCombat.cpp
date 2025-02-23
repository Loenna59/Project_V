// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/PlayerCombat.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayCharacter.h"
#include "Player/PlayerAnimInstance.h"
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
}


// Called when the game starts
void UPlayerCombat::BeginPlay()
{
	Super::BeginPlay();
	
	cameraComp = me->anchoredCameraComp;
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
		break;
	}
}

void UPlayerCombat::OnAnchor()
{
	me->OnAnchoredMode(); 
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

	me->Fire(CalculateAnimToVector(), drawStrength / targetDrawStrength);

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

