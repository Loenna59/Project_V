// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Component/PlayerCameraSwitcher.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayCharacter.h"

// Sets default values for this component's properties
UPlayerCameraSwitcher::UPlayerCameraSwitcher()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerCameraSwitcher::BeginPlay()
{
	Super::BeginPlay();

	springArmComp = me->springArmComp;
	cameraComp = me->cameraComp;
	anchoredSpringArmComp = me->anchoredSpringArmComp;
	anchoredCameraComp = me->anchoredCameraComp;
	transitionSpringArmComp = me->transitionSpringArmComp;
	transitionCameraComp = me->transitionCameraComp;
}

// Called every frame
void UPlayerCameraSwitcher::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

void UPlayerCameraSwitcher::OnChangedCameraMode(EPlayerCameraMode mode)
{
	Super::OnChangedCameraMode(mode);

	switch (mode)
	{
	case EPlayerCameraMode::Default:
		anchoredCameraComp->SetActive(false);
		transitionCameraComp->SetActive(true);

		anchoredCameraComp->SetFieldOfView(maxFOV);

		transitionSpringArmComp->bUsePawnControlRotation = springArmComp->bUsePawnControlRotation;
		transitionSpringArmComp->bEnableCameraLag = springArmComp->bEnableCameraLag;
		transitionSpringArmComp->bEnableCameraRotationLag = springArmComp->bEnableCameraRotationLag;
		transitionSpringArmComp->CameraLagSpeed = springArmComp->CameraLagSpeed;
		transitionSpringArmComp->CameraRotationLagSpeed = springArmComp->CameraRotationLagSpeed;

		targetBlendCameraAlpha = 0.f;
		break;
	case EPlayerCameraMode::Anchored:
	case EPlayerCameraMode::Focus:
		cameraComp->SetActive(false);
		transitionCameraComp->SetActive(true);

		transitionSpringArmComp->bUsePawnControlRotation = anchoredSpringArmComp->bUsePawnControlRotation;
		transitionSpringArmComp->bEnableCameraLag = anchoredSpringArmComp->bEnableCameraLag;
		transitionSpringArmComp->bEnableCameraRotationLag = anchoredSpringArmComp->bEnableCameraRotationLag;
		transitionSpringArmComp->CameraLagSpeed = anchoredSpringArmComp->CameraLagSpeed;
		transitionSpringArmComp->CameraRotationLagSpeed = anchoredSpringArmComp->CameraRotationLagSpeed;
	
		targetBlendCameraAlpha = 1.f;
		break;
	}
}

void UPlayerCameraSwitcher::SetCameraSlowMode(bool bActive)
{
	if (bActive)
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

