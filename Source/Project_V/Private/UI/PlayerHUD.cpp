// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/PlayerUI.h"

APlayerHUD::APlayerHUD()
{
	ConstructorHelpers::FClassFinder<UPlayerUI> tmpUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_PlayerUI.WBP_PlayerUI_C'"));

	if (tmpUI.Succeeded())
	{
		playerUIFactory = tmpUI.Class;
	}
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* ui = CreateWidget<UUserWidget>(GetWorld(), playerUIFactory);
	
	if (ui)
	{
		playerUI = Cast<UPlayerUI>(ui);
	
		if (playerUI)
		{
			playerUI->AddToViewport();
		}
	}
}
