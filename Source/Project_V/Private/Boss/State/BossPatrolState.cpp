// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossPatrolState.h"

#include "NavigationSystem.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	DistanceAlongSpline += GetWorld()->DeltaTimeSeconds * Boss->BossSpeed;
	if (DistanceAlongSpline > Boss->GetSplineComponent()->GetSplineLength())
	{
		DistanceAlongSpline = 0;
	}

	FVector TargetLoc = Boss->GetSplineComponent()->GetLocationAtDistanceAlongSpline(DistanceAlongSpline,ESplineCoordinateSpace::World);
	FVector Direction = (TargetLoc - Boss->GetActorLocation()).GetSafeNormal();
	Boss->AddMovementInput(Direction,1.0f);

	float TargetRotYaw = Boss->GetSplineComponent()->GetRotationAtDistanceAlongSpline(DistanceAlongSpline,ESplineCoordinateSpace::World).Yaw;
	float NewYaw = FMath::FInterpTo(Boss->GetActorRotation().Yaw,TargetRotYaw,GetWorld()->DeltaTimeSeconds,1.0f);
	FRotator NewRot = FRotator(Boss->GetActorRotation().Pitch,NewYaw,Boss->GetActorRotation().Roll);
	Boss->SetActorRotation(NewRot);
}

