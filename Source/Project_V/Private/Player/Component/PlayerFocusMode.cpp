// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/PlayerFocusMode.h"

#include "EnhancedInputComponent.h"
#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FocusDome.h"
#include "Player/PlayCharacter.h"
#include "UI/PlayerUI.h"

// Sets default values for this component's properties
UPlayerFocusMode::UPlayerFocusMode()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_focus(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerFocusMode.IA_PlayerFocusMode'"));

	if (tmp_ia_focus.Succeeded())
	{
		ia_focus = tmp_ia_focus.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmp_ia_toggle(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerToggle.IA_PlayerToggle'"));

	if (tmp_ia_toggle.Succeeded())
	{
		ia_toggle = tmp_ia_toggle.Object;
	}

	ConstructorHelpers::FClassFinder<AFocusDome> tmp_dome(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_FocusDome.BP_FocusDome_C'"));

	if (tmp_dome.Succeeded())
	{
		domeFactory = tmp_dome.Class;
	}
}


// Called when the game starts
void UPlayerFocusMode::BeginPlay()
{
	Super::BeginPlay();

	if ((focusDome = GetWorld()->SpawnActor<AFocusDome>(domeFactory)))
	{
		focusDome->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		focusDome->SetActorRelativeLocation(FVector(0, 0, -200));
	}

	Boss = Cast<AThunderJaw>(UGameplayStatics::GetActorOfClass(GetWorld(),AThunderJaw::StaticClass()));
}

void UPlayerFocusMode::SetupInputBinding(class UEnhancedInputComponent* input)
{
	Super::SetupInputBinding(input);

	input->BindAction(ia_focus, ETriggerEvent::Triggered, this, &UPlayerFocusMode::OnFocusOrScan);
	input->BindAction(ia_focus, ETriggerEvent::Completed, this, &UPlayerFocusMode::EndFocusOrScan);
	input->BindAction(ia_toggle, ETriggerEvent::Started, this, &UPlayerFocusMode::OnToggleTracking);
}

void UPlayerFocusMode::OnFocusOrScan(const FInputActionValue& actionValue)
{
	if (!focusDome)
	{
		return;
	}
	
	if (me->GetPlayerCameraMode() == EPlayerCameraMode::Focus)
	{
		SetVisibleFocusMode(false);
		focusDome->Deactivate();
		onEventCameraModeChanged.Execute(EPlayerCameraMode::Default);
		return;
	}

	if (focusPressingTime > focusModeThreshold)
	{
		if (me)
		{
			if (IsValid(me->ui))
			{
				me->ui->OnFocusModeToggle(bIsTracking);
			}
		}
		
		focusDome->Activate();
		onEventCameraModeChanged.Execute(EPlayerCameraMode::Focus);
	}
	
	focusPressingTime += GetWorld()->DeltaTimeSeconds;
}

void UPlayerFocusMode::EndFocusOrScan()
{
	EPlayerCameraMode currentMode = me->GetPlayerCameraMode();
	if (currentMode != EPlayerCameraMode::Focus && focusPressingTime < focusModeThreshold)
	{
		focusPressingTime = 0;
		return;
	}
	
	focusPressingTime = 0;
	me->SetPlayerCameraMode(currentMode == EPlayerCameraMode::Focus? EPlayerCameraMode::Default : EPlayerCameraMode::Focus);

	SetVisibleFocusMode(currentMode != EPlayerCameraMode::Focus);
}

void UPlayerFocusMode::OnToggleTracking()
{
	bIsTracking = !bIsTracking;
	
	if (me)
	{
		if (IsValid(me->ui))
		{
			me->ui->OnFocusModeToggle(bIsTracking);
		}
	}
}

void UPlayerFocusMode::SetVisibleFocusMode(bool visible)
{
	// PrintLogFunc(TEXT("SetVisibleFocusMode %s"), (visible? TEXT("true") : TEXT("false")));
	if (Boss)
	{
		Boss->ChangeToFocusModeMat(visible);
		Boss->SetVisibilitySpline(bIsTracking || visible);
	}
}

void UPlayerFocusMode::OnChangedCameraMode(EPlayerCameraMode mode)
{
	Super::OnChangedCameraMode(mode);

	if (mode != EPlayerCameraMode::Focus)
	{
		focusDome->Deactivate();
	}
}
