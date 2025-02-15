// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossCombatState.h"

#include "Project_V.h"
#include "Boss/MachineGun.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayCharacter.h"


void UBossCombatState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
}

void UBossCombatState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);

	// 몸을 돌리는 중이면 공격하지 않음
	if (Boss->bIsRotateBody)
	{
		RotateToTarget(Boss,1.0);
		Boss->GetBossAnimInstance()->OnPlayTurnMontage();
		
		// enemy의 시야각 까지 돌렸으면 false로 변경
		if (Boss->GetBossAIController()->FacingDot > 0.7)
		{
			Boss->bIsRotateBody = false;
		}
	}
	// 몸을 다돌린 후 다시 플레이어가 앞에 있는지 확인
	// 이 후 플레이어와의 거리를 계산해서 거리에 따른 공격
	else
	{
		if (Boss->GetBossAIController()->FacingDot > 0)
		{
			Attack(Boss,DeltaTime);
		}
		else
		{
			Boss->bIsRotateBody = true;
		}
	}
}

void UBossCombatState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}

void UBossCombatState::RotateToTarget(AThunderJaw* Boss, float InterpSpeed)
{
	// 타겟 위치로 몸을 돌리는 함수
	
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), Boss->GetAloy()->GetActorLocation());
	float NewYaw = UKismetMathLibrary::FInterpTo(Boss->GetActorRotation().Yaw,LookAtRot.Yaw, GetWorld()->GetDeltaSeconds(),InterpSpeed);
	Boss->SetActorRotation(FRotator(0,NewYaw,0));
}

void UBossCombatState::Attack(AThunderJaw* Boss, float DeltaTime)
{
	if (bIsDelay)
	{
		return;
	}
	
	PatternCurrentTime += DeltaTime;
	if (PatternCurrentTime >= PatternTime)
	{
		PatternCurrentTime = 0;
		StartPattern(Boss);
	}
	else
	{
		switch (UsingPattern)
		{
		case EAttackPattern::Charge:
			Charge(Boss);
			break;
		case EAttackPattern::MachineGun:
			UseMachineGun(Boss);
			break;
		case EAttackPattern::DiscLauncher:
			UseDiscLauncher(Boss);
			break;
		case EAttackPattern::MouseLaser:
			UseMouseLaser(Boss);
			break;
		default:
			break;
		}
	}

}

void UBossCombatState::StartPattern(AThunderJaw* Boss)
{
	bIsDelay = true;
	TWeakObjectPtr<AThunderJaw> WeakBoss = Boss;
	GetWorld()->GetTimerManager().SetTimer(PatternTimerHandle,
		[this,WeakBoss]()
		{
			if (WeakBoss.IsValid())
			{
				PatternDelayEnd(WeakBoss.Get());
			}
		},PatternDelay,false);
}

void UBossCombatState::PatternDelayEnd(AThunderJaw* Boss)
{
	bIsDelay = false;
	GetWorld()->GetTimerManager().ClearTimer(PatternTimerHandle);
	ChooseRandomPattern(Boss);
}

void UBossCombatState::ChooseRandomPattern(AThunderJaw* Boss)
{
	float Dist = Boss->GetBossAIController()->DistanceFromTarget;

	if (Dist <= Boss->MeleeAttackDist)
	{
		int randomNum = FMath::RandRange(0,0);
		if (randomNum == 0)
		{
			UsingPattern = EAttackPattern::Charge;
			PatternTime = ChargePatternTime;
		}
		else if (randomNum == 1)
		{
			UsingPattern = EAttackPattern::Tail;
			PatternTime = TailPatternTime;
		}
	}
	else
	{
		int randomNum = FMath::RandRange(2,2);
		if (randomNum == 2)
		{
			UsingPattern = EAttackPattern::MachineGun;
			PatternTime = MachineGunPatternTime;		
		}
		else if (randomNum == 3)
		{
			UsingPattern = EAttackPattern::DiscLauncher;
			PatternTime = DiscLauncherPatternTime;
		}
		else if (randomNum == 4)
		{
			UsingPattern = EAttackPattern::MouseLaser;
			PatternTime = MouseLaserPatternTime;
		}
	}
}

void UBossCombatState::Charge(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Charge"));
	Boss->GetBossAnimInstance()->OnPlayChargeMontage();
}

void UBossCombatState::UseMachineGun(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using Machine Gun"));
	FTransform Lt = Boss->GetLMachineGun()->FirePos->GetComponentTransform();
	FTransform Rt = Boss->GetRMachineGun()->FirePos->GetComponentTransform();

	// 회전하면서 쏠 때 timer에 loop로 처리하면 위치값이 업데이트 안되는 현상발생
	// timer를 사용하지 않고 직접 time을 받아서 사용하도록 함
	MachineGunDelayCurrentTime += GetWorld()->GetDeltaSeconds();
	if (MachineGunDelayCurrentTime > MachineGunDelay)
	{
		MachineGunDelayCurrentTime = 0;
		Boss->GetLMachineGun()->CreateBullet(Lt);
		Boss->GetRMachineGun()->CreateBullet(Rt);
	}
	
	RotateToTarget(Boss,0.7);

	if (Boss->GetBossAIController()->FacingDot < 0.85)
		Boss->GetBossAnimInstance()->OnPlayTurnMontage();
}

void UBossCombatState::UseDiscLauncher(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using DiscLauncher"));
}

void UBossCombatState::UseMouseLaser(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using MouseLaser"));
}
