// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossLookOutState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAnimInstance.h"

void UBossLookOutState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->ChangeEyeColor(FLinearColor(1,0.844786,0.016844),40);
	Boss->GetBossAnimInstance()->OnPlayRageMontage();
}

void UBossLookOutState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	// TODO
	// 마지막으로 감지된 곳을 저장한 후 해당 장소 위주로 패트롤
	// 패트롤 하다 timer를 통해 Radar패턴 실행
}

void UBossLookOutState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}
