// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossCombatState.h"

#include "Project_V.h"
#include "Boss/MachineGun.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayCharacter.h"



void UBossCombatState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
}

void UBossCombatState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	if (Boss->bIsRotateBody)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), Boss->GetAloy()->GetActorLocation());
		float NewYaw = UKismetMathLibrary::FInterpTo(Boss->GetActorRotation().Yaw,LookAtRot.Yaw,DeltaTime,1.0);
		Boss->SetActorRotation(FRotator(0,NewYaw,0));

		// enemy의 시야각 까지 돌렸으면 false로 변경
		if (Boss->GetBossAIController()->FacingDot > 0.7)
		{
			Boss->bIsRotateBody = false;
		}
	}
	else
	{
		if (Boss->GetBossAIController()->FacingDot > 0)
		{
			float dist = Boss->GetBossAIController()->DistanceFromTarget;
			if (!Boss->bIsAttack)
			{
			}
			if (dist <= Boss->MeleeAttackDist)
			{
				PRINTLOG(TEXT("boss melee attack"));
			}
			else if (dist > Boss->MeleeAttackDist)
			{
				ChooseRandomRangeAttack(Boss);
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

void UBossCombatState::ChooseRandomRangeAttack(AThunderJaw* Boss)
{
	Boss->bIsAttack = true;
	int randomNum = FMath::RandRange(1,1);
	if (randomNum == 1)
	{
		UseMachineGun(Boss);
	}
	else if (randomNum == 2)
	{
		UseDiscLauncher(Boss);
	}
	else if (randomNum == 3)
	{
		UseMouseLaser(Boss);
	}
}

void UBossCombatState::UseMachineGun(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using Machine Gun"));

	Boss->GetLMachineGun()->CreateBullet(Boss->GetLMachineGun()->FirePos->GetComponentTransform());
	Boss->GetRMachineGun()->CreateBullet(Boss->GetRMachineGun()->FirePos->GetComponentTransform());
}

void UBossCombatState::UseDiscLauncher(AThunderJaw* Boss)
{
}

void UBossCombatState::UseMouseLaser(AThunderJaw* Boss)
{
}
