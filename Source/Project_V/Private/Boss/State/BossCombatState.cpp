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
			float dist = Boss->GetBossAIController()->DistanceFromTarget;

			if (dist <= Boss->MeleeAttackDist)
			{
				PRINTLOG(TEXT("boss melee attack"));
			}
			else if (dist > Boss->MeleeAttackDist)
			{
				RangeAttack(Boss,DeltaTime);
			}
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
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), Boss->GetAloy()->GetActorLocation());
	float NewYaw = UKismetMathLibrary::FInterpTo(Boss->GetActorRotation().Yaw,LookAtRot.Yaw, GetWorld()->GetDeltaSeconds(),InterpSpeed);
	Boss->SetActorRotation(FRotator(0,NewYaw,0));
}

void UBossCombatState::RangeAttack(AThunderJaw* Boss, float DeltaTime)
{
	PatternCurrentTime += DeltaTime;
	if (PatternCurrentTime > PatternTime)
	{
		PatternCurrentTime = 0;
		ChooseRandomRangeAttack(Boss);
	}

	switch (UsingWeapon)
	{
	case ERangeWeapon::MachineGun:
		UseMachineGun(Boss);
		break;
	case ERangeWeapon::DiscLauncher:
		UseDiscLauncher(Boss);
		break;
	case ERangeWeapon::MouseLaser:
		UseMouseLaser(Boss);
		break;
	default:
		break;
	}
}

void UBossCombatState::ChooseRandomRangeAttack(AThunderJaw* Boss)
{
	int randomNum = FMath::RandRange(1,1);
	if (randomNum == 1)
	{
		UsingWeapon = ERangeWeapon::MachineGun;
		PatternTime = MachineGunPatternTime;		
	}
	else if (randomNum == 2)
	{
		UsingWeapon = ERangeWeapon::DiscLauncher;
		PatternTime = DiscLauncherPatternTime;
	}
	else if (randomNum == 3)
	{
		UsingWeapon = ERangeWeapon::MouseLaser;
		PatternTime = MouseLaserPatternTime;
	}
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
