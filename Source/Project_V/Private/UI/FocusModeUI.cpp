// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FocusModeUI.h"

void UFocusModeUI::OnToggle(bool isTracking)
{
	if (isTracking)
	{
		TrackingText->SetText(FText::FromString("[F] Deselect Track"));
		return;
	}

	TrackingText->SetText(FText::FromString("[F] Highlight Track"));
}
