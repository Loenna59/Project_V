// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UBossHPUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerHPUI();
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;
	
	UPROPERTY()
	class AThunderJaw* Boss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHP;
};
