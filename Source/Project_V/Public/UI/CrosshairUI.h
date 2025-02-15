// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UCrosshairUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UImage* Circle;

	void UpdateCircle(float strength);
};
