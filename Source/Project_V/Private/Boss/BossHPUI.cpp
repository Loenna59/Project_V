// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossHPUI.h"

#include "Boss/ThunderJaw.h"
#include "Components/ProgressBar.h"

void UBossHPUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBossHPUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Boss = Cast<AThunderJaw>(GetOuter());
	if (Boss)
	{
		CurrentHP = Boss->CurrentHP;
		MaxHP = Boss->MaxHP;
	}
}

void UBossHPUI::SetPlayerHPUI()
{
	float per = CurrentHP / MaxHP;
	HPBar->SetPercent(per);
}
