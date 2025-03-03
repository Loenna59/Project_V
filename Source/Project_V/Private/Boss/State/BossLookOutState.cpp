// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossLookOutState.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Engine/OverlapResult.h"
#include "Navigation/PathFollowingComponent.h"
#include "Player/PlayCharacter.h"

void UBossLookOutState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->ChangeEyeColor(FLinearColor(1,0.844786,0.016844),40);
	Boss->GetBossAnimInstance()->OnPlayRageMontage();

	if (GetWorld()->GetTimerManager().TimerExists(RadarTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RadarTimerHandle);
	}
	
	GetWorld()->GetTimerManager().SetTimer(RadarTimerHandle,[this,Boss]()
	{
		bUsingRadar = true;
		Boss->GetBossAnimInstance()->OnPlayRadarMontage();
		Boss->GetBossAIController()->StopMovement();
	},RadarUsingTime,true);
}

void UBossLookOutState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);

	if (Boss->GetBossAnimInstance()->CurrentPlayingMontage != nullptr)
	{
		if (Boss->GetBossAnimInstance()->CurrentPlayingMontage->GetName() == TEXT("RageMontage") || Boss->GetBossAnimInstance()->CurrentPlayingMontage->GetName() == TEXT("TurnMontage"))
		{
			return;
		}
	}

	if (Boss->GetBossAIController()->bDebugMode)
	{
		DrawDebugBox(GetWorld(),Boss->GetBossAIController()->DetectedLocation,FVector(50),FColor::Red);
		DrawDebugBox(GetWorld(),FSM->RandomLocation,FVector(50),FColor::Green);
	}

	if (bUsingRadar)
	{
		if (bOnSphereTrace)
		{
			MakeSphereTrace(Boss,FSM);
		}
		else
		{
			Radar(Boss,FSM);
		}
	}
	else
	{
		PatrolToDetectLocation(Boss,FSM);
	}
}

void UBossLookOutState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
	Boss->GetBossAIController()->StopMovement();
	
	if (GetWorld()->GetTimerManager().IsTimerActive(RadarTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RadarTimerHandle);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(SencingTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(SencingTimerHandle);
	}
}

void UBossLookOutState::PatrolToDetectLocation(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	// detect된 위치로 이동
	if (!FSM->bIsArrivedDetectedLocation)
	{
		EPathFollowingRequestResult::Type result = Boss->GetBossAIController()->MoveToLocation(Boss->GetBossAIController()->DetectedLocation);
		// 해당 위치로 도착했으면
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal || result == EPathFollowingRequestResult::Type::Failed)
		{
			// 해당 위치를 기준으로 정찰
			FSM->GetRandomLocationFromNavMesh(Boss->GetBossAIController()->DetectedLocation, Boss->PatrolDist,FSM->RandomLocation);
			FSM->bIsArrivedDetectedLocation = true;
		}
	}
	else
	{
		EPathFollowingRequestResult::Type result = Boss->GetBossAIController()->MoveToLocation(FSM->RandomLocation);
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal || result == EPathFollowingRequestResult::Type::Failed)
		{
			FSM->GetRandomLocationFromNavMesh(Boss->GetBossAIController()->DetectedLocation, Boss->PatrolDist,FSM->RandomLocation);
		}
	}
}

void UBossLookOutState::Radar(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Boss->GetBossAIController()->StopMovement();

	if (GetWorld()->GetTimerManager().IsTimerActive(SencingTimerHandle))
	{
		return;
	}
	
	PRINTLOG(TEXT("sencing timer on"));
	GetWorld()->GetTimerManager().SetTimer(SencingTimerHandle,[this]()
	{
		bOnSphereTrace = true;
	},SencingTime,false);
}

void UBossLookOutState::MakeSphereTrace(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	FCollisionQueryParams Params;
	TArray<FOverlapResult> OverlapResults;

	bool bOverlap = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Boss->GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(Boss->GetBossAIController()->DetectDist + 100),
		Params);

	FColor SphereColor = FColor::Blue;
	if (bOverlap)
	{
		for (const FOverlapResult& overlap : OverlapResults)
		{
			auto* player = Cast<APlayCharacter>(overlap.GetActor());
			if (player)
			{
				PRINTLOG(TEXT("Detect Success"));
				SphereColor = FColor::Red;
				bUsingRadar = false;
				bOnSphereTrace = false;
				FSM->ChangeBossState(EBossState::Combat);
			}
			else
			{
				bUsingRadar = false;
				bOnSphereTrace = false;
			}
		}
	}

	if (Boss->GetBossAIController()->bDebugMode)
	{
		DrawDebugSphere(GetWorld(),Boss->GetActorLocation(),Boss->GetBossAIController()->DetectDist + 100,32,SphereColor);
	}
}
