// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossHPUI.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UBossHPUI::NativeConstruct()
{
	Super::NativeConstruct();
	Boss = Cast<AThunderJaw>(UGameplayStatics::GetActorOfClass(GetWorld(),AThunderJaw::StaticClass()));

}

void UBossHPUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//PRINT_CALLINFO();
	SetPlayerHPUI();
}

void UBossHPUI::SetPlayerHPUI()
{
	if (Boss)
	{
		CurrentHP = Boss->CurrentHP;
		MaxHP = Boss->MaxHP;
	}
	float per = CurrentHP / MaxHP;
	//PRINTLOG(TEXT("%f"),per);
	HPBar->SetPercent(per);
}
