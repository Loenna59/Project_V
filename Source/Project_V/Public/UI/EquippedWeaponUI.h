// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquippedWeaponUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UEquippedWeaponUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UImage* WeaponBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UImage* Tripcaster;

	void SetVisible(bool isBase);
};
