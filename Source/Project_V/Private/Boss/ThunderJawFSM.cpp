

#include "Boss/ThunderJawFSM.h"

#include "NavigationSystem.h"
#include "Boss/ThunderJaw.h"
#include "Boss/State/BossCombatState.h"
#include "Boss/State/BossBaseState.h"
#include "Boss/State/BossDamageState.h"
#include "Boss/State/BossDieState.h"
#include "Boss/State/BossIdleState.h"
#include "Boss/State/BossLookOutState.h"
#include "Boss/State/BossPatrolState.h"


UThunderJawFSM::UThunderJawFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UThunderJawFSM::BeginPlay()
{
	Super::BeginPlay();
	Boss = Cast<AThunderJaw>(GetOwner());

	InitStatePool();
	
	CurrentState = StatePool[EBossState::Patrol];
	CurrentState->Enter(Boss,this);
}


void UThunderJawFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Boss->bIsLSEnd || Boss->bDie)
	{
		return;
	}
	
	if (CurrentState)
	{
		CurrentState->Update(Boss,this,DeltaTime);
	}
}

void UThunderJawFSM::InitStatePool()
{
	// Idle State
	StatePool.Add(EBossState::Idle, NewObject<UBossIdleState>(this, UBossIdleState::StaticClass()));
	StatePool[EBossState::Idle]->BossState = EBossState::Idle;
	// Patrol State
	StatePool.Add(EBossState::Patrol, NewObject<UBossPatrolState>(this,UBossPatrolState::StaticClass()));
	StatePool[EBossState::Patrol]->BossState = EBossState::Patrol;
	// Combat State
	StatePool.Add(EBossState::Combat, NewObject<UBossCombatState>(this, UBossCombatState::StaticClass()));
	StatePool[EBossState::Combat]->BossState = EBossState::Combat;
	// LookOut State
	StatePool.Add(EBossState::LookOut, NewObject<UBossLookOutState>(this,UBossLookOutState::StaticClass()));
	StatePool[EBossState::LookOut]->BossState = EBossState::LookOut;
	// Damage State
	StatePool.Add(EBossState::Damage,NewObject<UBossDamageState>(this,UBossDamageState::StaticClass()));
	StatePool[EBossState::Damage]->BossState = EBossState::Damage;
	// Die State
	StatePool.Add(EBossState::Die, NewObject<UBossDieState>(this, UBossDieState::StaticClass()));
	StatePool[EBossState::Die]->BossState = EBossState::Die;
}

void UThunderJawFSM::ChangeBossState(EBossState BossState)
{
	if (CurrentState->BossState == BossState || CurrentState->BossState == EBossState::Die)
	{
		return;
	}
	
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

bool UThunderJawFSM::GetRandomLocationFromNavMesh(FVector CenterLocation, float Radius, FVector& Dest)
{
	auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation Loc;
	bool bResult = NavSystem->GetRandomReachablePointInRadius(CenterLocation,Radius,Loc);
	float Dist = FVector::Distance(Boss->GetActorLocation(),Loc.Location);


	while (Dist < Radius / 2.0)
	{
		bResult = NavSystem->GetRandomReachablePointInRadius(CenterLocation,Radius,Loc);
		Dist = FVector::Distance(Boss->GetActorLocation(),Loc.Location);
	}
	
	Dest = Loc.Location;
	PRINTLOG(TEXT("randomLoc : %f,%f,%f"),Dest.X,Dest.Y,Dest.Z);
	return bResult;
}

