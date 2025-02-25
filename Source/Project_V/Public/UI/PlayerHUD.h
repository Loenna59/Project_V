// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

	APlayerHUD();

	virtual void BeginPlay() override;

	TSubclassOf<class UPlayerUI> playerUIFactory;
	

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UPlayerUI* playerUI;
	
	class UPlayerUI* GetPlayerUI() const
	{
		return playerUI;
	}
};
