// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStateUI.h"

#include "Components/BackgroundBlur.h"
#include "Components/TextBlock.h"

void UGameStateUI::SetVisibleAndChangeGameStateText(bool gameClear)
{
	BGBlur->SetVisibility(ESlateVisibility::Visible);
	GameStateText->SetVisibility(ESlateVisibility::Visible);
	if (gameClear)
	{
		GameStateText->SetText(FText::FromString(TEXT("Game Clear")));
	}
	else
	{
		GameStateText->SetText(FText::FromString(TEXT("Game Over")));
	}
}
