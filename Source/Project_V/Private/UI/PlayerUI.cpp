// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"

#include "UI/CrosshairUI.h"

void UPlayerUI::SetVisibleUI(CameraMode mode)
{
	Crosshair->SetVisibility(mode == CameraMode::Anchored? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
