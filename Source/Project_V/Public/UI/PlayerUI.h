// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/PlayerCameraMode.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UCrosshairUI* Crosshair;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UPlayerHPUI* HealthUI;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UFocusModeUI* FocusModeUI;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UEquippedWeaponUI* EquippedWeaponUI;

	void SetVisibleUI(EPlayerCameraMode mode);

	void SetHealthUI(float current, float max);
	
	void ChangeEquippedWeaponUI(bool isBow);
};
