// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStateUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UGameStateUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetVisibleAndChangeGameStateText(bool gameClear);
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameStateText;
	UPROPERTY(meta = (BindWidget))
	class UBackgroundBlur* BGBlur;
	
};
