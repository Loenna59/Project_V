// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FocusModeUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UFocusModeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* TrackingText;

	void OnToggle(bool isTracking);
	
};
