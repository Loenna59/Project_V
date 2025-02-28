// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageUI.h"

#include "Components/TextBlock.h"

void UDamageUI::SetDamageText(int Damage)
{
	DamageText->SetText(FText::AsNumber(Damage));
}

void UDamageUI::PlayAnimAndRemoveDamageUI()
{
	if (MoveAnim)
	{
		PlayAnimation(MoveAnim);
	}
	FTimerHandle DamageUITimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DamageUITimerHandle,[this]()
	{
		RemoveFromParent();
	},1.0,false);
}
