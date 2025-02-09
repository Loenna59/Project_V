

#include "Boss/ThunderJawFSM.h"

#include "Boss/ThunderJaw.h"
#include "Boss/State/BossAttackState.h"
#include "Boss/State/BossBaseState.h"
#include "Boss/State/BossIdleState.h"
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

	if (CurrentState)
	{
		CurrentState->Update(Boss,this,DeltaTime);
	}
}

void UThunderJawFSM::InitStatePool()
{
	// Idle State
	StatePool.Add(EBossState::Idle, NewObject<UBossIdleState>(this, UBossIdleState::StaticClass()));
	// Patrol State
	StatePool.Add(EBossState::Patrol, NewObject<UBossPatrolState>(this,UBossPatrolState::StaticClass()));
	// Attack State
	StatePool.Add(EBossState::Attack, NewObject<UBossAttackState>(this, UBossAttackState::StaticClass()));
}

void UThunderJawFSM::ChangeState(EBossState BossState)
{
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



