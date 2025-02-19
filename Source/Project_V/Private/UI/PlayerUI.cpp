// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"

#include "Player/PlayerCameraMode.h"
#include "UI/CrosshairUI.h"
#include "UI/FocusModeUI.h"
#include "UI/PlayerHPUI.h"

void UPlayerUI::SetVisibleUI(EPlayerCameraMode mode)
{
	Crosshair->SetVisibility(mode == EPlayerCameraMode::Anchored? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	FocusModeUI->SetVisibility(mode == EPlayerCameraMode::Focus? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UPlayerUI::SetHealthUI(float current, float max)
{
	HealthUI->SetText(current, max);
	HealthUI->SetProgress(current, max);
}
