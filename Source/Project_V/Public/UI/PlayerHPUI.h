// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayerHPUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UProgressBar* ProgressBar;

	void SetText(float current, float max);
	
	void SetProgress(float current, float max);
	
};
