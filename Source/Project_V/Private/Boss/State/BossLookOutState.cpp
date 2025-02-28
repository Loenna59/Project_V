// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossLookOutState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Navigation/PathFollowingComponent.h"

void UBossLookOutState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->ChangeEyeColor(FLinearColor(1,0.844786,0.016844),40);
	Boss->GetBossAnimInstance()->OnPlayRageMontage();
}

void UBossLookOutState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);

	if (Boss->GetBossAIController()->bDebugMode)
	{
		DrawDebugBox(GetWorld(),Boss->GetBossAIController()->DetectedLocation,FVector(50),FColor::Red);
		DrawDebugBox(GetWorld(),FSM->RandomLocation,FVector(50),FColor::Green);
	}
	

	if (Boss->GetBossAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}
	
	// detect된 위치로 이동
	if (!FSM->bIsArrivedDetectedLocation)
	{
		auto result = Boss->GetBossAIController()->MoveToLocation(Boss->GetBossAIController()->DetectedLocation);
		// 해당 위치로 도착했으면
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Type::Failed)
		{
			// 해당 위치를 기준으로 정찰
			FSM->GetRandomLocationFromNavMesh(Boss->GetBossAIController()->DetectedLocation, Boss->PatrolDist,FSM->RandomLocation);
			FSM->bIsArrivedDetectedLocation = true;
		}
	}
	else
	{
		auto result = Boss->GetBossAIController()->MoveToLocation(FSM->RandomLocation);
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Type::Failed)
		{
			FSM->GetRandomLocationFromNavMesh(Boss->GetBossAIController()->DetectedLocation, Boss->PatrolDist,FSM->RandomLocation);
		}
	}
}

void UBossLookOutState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
	Boss->GetBossAIController()->StopMovement();
}
