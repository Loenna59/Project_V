// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHPUI.h"

#include "Project_V.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHPUI::SetText(float current, float max)
{
	FString str = FString::Printf(TEXT("%d / %d"), FMath::FloorToInt(current), FMath::FloorToInt(max));
	HealthText->SetText(FText::FromString(str));
}

void UPlayerHPUI::SetProgress(float current, float max)
{
	ProgressBar->SetPercent(current / max);
}
