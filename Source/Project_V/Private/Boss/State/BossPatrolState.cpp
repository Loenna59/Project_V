// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossPatrolState.h"

#include "NavigationSystem.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "Player/PlayCharacter.h"

void UBossPatrolState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->ChangeEyeColor(FLinearColor(0,0.14,1),2000);
	FSM->GetRandomLocationFromNavMesh(Boss->GetActorLocation(), Boss->PatrolDist,FSM->RandomLocation);
}

void UBossPatrolState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	
	Patrol(Boss,FSM);
}

void UBossPatrolState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
	Boss->GetBossAIController()->StopMovement();
}

void UBossPatrolState::Patrol(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	if (!FSM->bIsArriveDestLoc)
	{
		auto result = Boss->GetBossAIController()->MoveToLocation(FSM->RandomLocation);
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Type::Failed)
		{
			FSM->GetRandomLocationFromNavMesh(Boss->GetActorLocation(), Boss->PatrolDist,FSM->RandomLocation);
			FSM->bIsArriveDestLoc = true;
			FSM->bIsRotateEnd = false;
		}
	}

	if (!FSM->bIsRotateEnd)
	{
		Boss->GetBossAIController()->StopMovement();
		RotateToTarget(Boss,FSM,1.0f);
		Boss->GetBossAnimInstance()->OnPlayTurnMontage();
	}
	else
	{
		FSM->bIsRotateEnd = true;
		FSM->bIsArriveDestLoc = false;
	}

	DrawDebugBox(GetWorld(),FSM->RandomLocation,FVector(50),FColor::Red);
}

void UBossPatrolState::RotateToTarget(AThunderJaw* Boss, UThunderJawFSM* FSM, float InterpSpeed)
{
	// 타겟 위치로 몸을 돌리는 함수
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), FSM->RandomLocation);
	float NewYaw = UKismetMathLibrary::FInterpTo(Boss->GetActorRotation().Yaw,LookAtRot.Yaw, GetWorld()->GetDeltaSeconds(),InterpSpeed);
	Boss->SetActorRotation(FRotator(0,NewYaw,0));
	// 현재 회전값과 목표 회전값의 차이 계산
	float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(Boss->GetActorRotation().Yaw, LookAtRot.Yaw));
	// 임계값보다 작으면 회전 완료로 간주
	FSM->bIsRotateEnd = YawDifference <= 18.0;
}


