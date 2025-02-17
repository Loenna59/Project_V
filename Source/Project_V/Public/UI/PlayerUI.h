// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

UENUM(BlueprintType)
enum class CameraMode : uint8
{
	Default,
	Anchored
};

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

	void SetVisibleUI(CameraMode mode);

	void SetHealthUI(float current, float max);
};
