// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossPatrolState.h"

#include "NavigationSystem.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Player/PlayCharacter.h"

void UBossPatrolState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->ChangeEyeColor(FLinearColor(0,0.14,1),500);
}

void UBossPatrolState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);
	
	auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FVector dest = Boss->GetAloy()->GetActorLocation();
	FPathFindingQuery query;
	FAIMoveRequest request;
	// 도착 영역 설정
	request.SetAcceptanceRadius(3.0);
	// 도착지 설정
	request.SetGoalLocation(dest);
	// 길찾기를 위한 쿼리 생성 (도착지에 도착할 수 있는지 질문 생성)
	Boss->GetBossAIController()->BuildPathfindingQuery(request,query);
	// 질문하기
	FPathFindingResult answer = NavSystem->FindPathSync(query);
	
	// 도착지를 못찾았으면 실행
	if (answer.Result != ENavigationQueryResult::Type::Success)
	{
		//PRINTLOG(TEXT("Didn't Find Target"));
		// 랜덤위치 설정하고 그리로 이동
		// 1. 랜덤위치로 이동
		auto result = Boss->GetBossAIController()->MoveToLocation(FSM->RandomLocation);
		// 2. 랜덤위치로 도착하면?
		// -> 랜덤위치 다시 설정
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal ||
			result == EPathFollowingRequestResult::Type::Failed)
		{
			FSM->GetRandomLocationFromNavMesh(Boss->GetActorLocation(), 2000.0f,FSM->RandomLocation);
		}
	}
}

void UBossPatrolState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
	Boss->GetBossAIController()->StopMovement();
}
