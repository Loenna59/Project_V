// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossCombatState.h"

#include "Project_V.h"
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
			if (dist <= Boss->MeleeAttackDist)
			{
				//UE_LOG(LogTemp,Warning,TEXT("boss melee attack"));
			}
			else if (dist > Boss->MeleeAttackDist)
			{
				//UE_LOG(LogTemp,Warning,TEXT("boss range attack"));
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
