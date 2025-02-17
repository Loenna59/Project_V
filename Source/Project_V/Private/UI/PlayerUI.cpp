// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"

#include "UI/CrosshairUI.h"
#include "UI/PlayerHPUI.h"

void UPlayerUI::SetVisibleUI(CameraMode mode)
{
	Crosshair->SetVisibility(mode == CameraMode::Anchored? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UPlayerUI::SetHealthUI(float current, float max)
{
	HealthUI->SetText(current, max);
	HealthUI->SetProgress(current, max);
}
