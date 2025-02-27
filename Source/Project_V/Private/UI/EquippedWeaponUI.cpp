// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EquippedWeaponUI.h"

#include "Components/Image.h"

void UEquippedWeaponUI::SetVisible(bool isBase)
{
	WeaponBase->SetVisibility(isBase? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	Tripcaster->SetVisibility(!isBase? ESlateVisibility::Visible: ESlateVisibility::Hidden);
}
