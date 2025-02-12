

#include "Boss/ThunderJawFSM.h"

#include "Boss/ThunderJaw.h"
#include "Boss/State/BossCombatState.h"
#include "Boss/State/BossBaseState.h"
#include "Boss/State/BossIdleState.h"
#include "Boss/State/BossPatrolState.h"
#include "GameFramework/CharacterMovementComponent.h"


UThunderJawFSM::UThunderJawFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UThunderJawFSM::BeginPlay()
{
	Super::BeginPlay();

	Boss = Cast<AThunderJaw>(GetOwner());
	InitStatePool();
	InitPatrolPoints();
	
	CurrentState = StatePool[EBossState::Patrol];
	CurrentState->Enter(Boss,this);
}


void UThunderJawFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->Update(Boss,this,DeltaTime);
	}
}

void UThunderJawFSM::InitStatePool()
{
	// Idle State
	StatePool.Add(EBossState::Idle, NewObject<UBossIdleState>(this, UBossIdleState::StaticClass()));
	StatePool[EBossState::Idle]->currentStateEnum = EBossState::Idle;
	// Patrol State
	StatePool.Add(EBossState::Patrol, NewObject<UBossPatrolState>(this,UBossPatrolState::StaticClass()));
	StatePool[EBossState::Patrol]->currentStateEnum = EBossState::Patrol;
	// Combat State
	StatePool.Add(EBossState::Combat, NewObject<UBossCombatState>(this, UBossCombatState::StaticClass()));
	StatePool[EBossState::Combat]->currentStateEnum = EBossState::Combat;
}

void UThunderJawFSM::InitPatrolPoints()
{
	PatrolPoints.Add(FVector(965.0,1310.0,0.0));
	PatrolPoints.Add(FVector(2335.0,1315.0,0.0));
	PatrolPoints.Add(FVector(2345.0,2395.0,0.0));
	PatrolPoints.Add(FVector(985.0,2405.0,0.0));
}

void UThunderJawFSM::ChangeBossState(EBossState BossState)
{
	UE_LOG(LogTemp,Warning,TEXT("Change State"));
	if (CurrentState->currentStateEnum == BossState)
		return;
	
	CurrentState->Exit(Boss, this);
	PrevState = CurrentState;
	
	if (StatePool.Contains(BossState))
	{
		CurrentState = StatePool[BossState];
		CurrentState->Enter(Boss, this);
	}
	else
	{
		check(false);
	}
}

UBossBaseState* UThunderJawFSM::GetCurrentState()
{
	return CurrentState;
}

UBossBaseState* UThunderJawFSM::GetPrevState()
{
	return PrevState;
}

void UThunderJawFSM::ChangePatrolTargetPoint()
{
	if (ArrivedTargetPoint)
	{
		//PrintLogFunc(TEXT("%d"),CurrentTargetPoint);
		ArrivedTargetPoint = false;
		if (CurrentTargetPoint >= PatrolPoints.Num() - 1)
		{
			CurrentTargetPoint = 0;
		}
		else
		{
			CurrentTargetPoint += 1;
		}
	}
}

void UThunderJawFSM::AdjustSpeed(float NewSpeed)
{
	if (Boss)
	{
		Boss->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}



