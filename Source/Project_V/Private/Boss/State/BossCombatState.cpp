// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossCombatState.h"

#include "Project_V.h"
#include "Boss/MachineGun.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayCharacter.h"


void UBossCombatState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
}

void UBossCombatState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);

	// 몸을 돌리는 중이면 공격하지 않음
	if (bIsRotateBody)
	{
		RotateToTarget(Boss,1.0);
		Boss->GetBossAnimInstance()->OnPlayMontage(EBossMontage::Turn);
		
		// enemy의 정면까지 돌렸으면 false로 변경
		if (Boss->GetBossAIController()->FacingDot > 0.9)
		{
			bIsRotateBody = false;
			StartChoosingPatternCycle(Boss);
		}
	}
	// 몸을 다돌린 후 다시 플레이어가 앞에 있는지 확인
	// 이 후 플레이어와의 거리를 계산해서 거리에 따른 공격
	else
	{
		if (Boss->GetBossAIController()->FacingDot > 0 ||
			UsingPattern != EAttackPattern::None)
		{
			if (bIsDelay)
			{
				return;
			}
			
			PatternCurrentTime += DeltaTime;
			if (PatternCurrentTime > PatternTime)
			{
				StartChoosingPatternCycle(Boss);
			}
			else
			{
				Attack(Boss,DeltaTime);
			}
		}
		else if (Boss->GetBossAIController()->FacingDot <= 0)
		{
			bIsRotateBody = true;
		}
	}
}

void UBossCombatState::Exit(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Exit(Boss, FSM);
}

void UBossCombatState::RotateToTarget(AThunderJaw* Boss, float InterpSpeed)
{
	// 타겟 위치로 몸을 돌리는 함수
	
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), Boss->GetAloy()->GetActorLocation());
	float NewYaw = UKismetMathLibrary::FInterpTo(Boss->GetActorRotation().Yaw,LookAtRot.Yaw, GetWorld()->GetDeltaSeconds(),InterpSpeed);
	Boss->SetActorRotation(FRotator(0,NewYaw,0));
}

void UBossCombatState::Attack(AThunderJaw* Boss, float DeltaTime)
{
	switch (UsingPattern)
	{
	case EAttackPattern::Charge:
		Charge(Boss);
		break;
	case EAttackPattern::Tail:
		Tail(Boss);
		break;
	case EAttackPattern::MachineGun:
		MachineGun(Boss);
		break;
	case EAttackPattern::DiscLauncher:
		DiscLauncher(Boss);
		break;
	case EAttackPattern::MouseLaser:
		MouseLaser(Boss);
		break;
	default:
		break;
	}
}

void UBossCombatState::StartChoosingPatternCycle(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("startpattern"));
	if (GetWorld()->GetTimerManager().IsTimerActive(PatternTimerHandle))
	{
		PRINTLOG(TEXT("timer exist"));
		GetWorld()->GetTimerManager().ClearTimer(PatternTimerHandle);
	}

	bIsDelay = true;

	// 패턴관련 변수 초기화
	UsingPattern = EAttackPattern::None;
	PatternCurrentTime = 0;
	PatternTime = 0;

	TWeakObjectPtr<AThunderJaw> WeakBoss = Boss;
	GetWorld()->GetTimerManager().SetTimer(PatternTimerHandle,
		[this,WeakBoss]()
		{
			if (WeakBoss.IsValid())
			{
				DelayEndBeforeChoosingPattern(WeakBoss.Get());
			}
		},PatternDelay,false);
}

void UBossCombatState::DelayEndBeforeChoosingPattern(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("PatternDelayEnd"));
	bIsDelay = false;
	ChargeFlag = false;
	
	ChooseRandomPattern(Boss);
}

void UBossCombatState::ChooseRandomPattern(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("ChooseRandomPattern"));
	float Dist = Boss->GetBossAIController()->DistanceFromTarget;

	if (Dist <= Boss->MeleeAttackDist)
	{
		int randomNum = FMath::RandRange(0,0);
		if (randomNum == 0)
		{
			UsingPattern = EAttackPattern::Charge;
			PatternTime = ChargePatternTime;
		}
		else if (randomNum == 1)
		{
			UsingPattern = EAttackPattern::Tail;
			PatternTime = TailPatternTime;
		}
	}
	else
	{
		int randomNum = FMath::RandRange(2,2);
		if (randomNum == 2)
		{
			UsingPattern = EAttackPattern::MachineGun;
			PatternTime = MachineGunPatternTime;		
		}
		else if (randomNum == 3)
		{
			UsingPattern = EAttackPattern::DiscLauncher;
			PatternTime = DiscLauncherPatternTime;
		}
		else if (randomNum == 4)
		{
			UsingPattern = EAttackPattern::MouseLaser;
			PatternTime = MouseLaserPatternTime;
		}
	}
}

void UBossCombatState::Charge(AThunderJaw* Boss)
{
	if (!ChargeFlag)
	{
		ChargeFlag = true;
		ChargeStart = false;
		PerposeLocation = (Boss->GetAloy()->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
		
		TWeakObjectPtr<AThunderJaw> WeakBoss = Boss;
		GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle,[this,WeakBoss]()
		{
			if (WeakBoss.IsValid())
			{
				PRINTLOG(TEXT("Using Charge"));
				ChargeStart = true;
			}
		},recoilTime,false);
	}

	if (!ChargeStart)
	{
		// 플레이어를 바라보면서 뒷걸음질을 하기 위해 움직이는 방향으로 회전하지 않게 막음
		Boss->GetCharacterMovement()->bOrientRotationToMovement = false;
		// 플레이어를 바라보게 몸을 돌려줌
		RotateToTarget(Boss,1.0f);
		Boss->AddMovementInput(-PerposeLocation, 5.0f);
	}
	else
	{
		Boss->GetCharacterMovement()->bOrientRotationToMovement = true;
		Boss->AddMovementInput(PerposeLocation,10.0f);
		Boss->GetBossAnimInstance()->OnPlayMontage(EBossMontage::Charge);
	}
}

void UBossCombatState::Tail(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using Tail"));
	Boss->GetBossAnimInstance()->OnPlayMontage(EBossMontage::Tail);
}

void UBossCombatState::MachineGun(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using Machine Gun"));
	FTransform Lt = Boss->GetLMachineGun()->FirePos->GetComponentTransform();
	FTransform Rt = Boss->GetRMachineGun()->FirePos->GetComponentTransform();
	FVector dir = (Boss->GetAloy()->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();

	// 회전하면서 쏠 때 timer에 loop로 처리하면 위치값이 업데이트 안되는 현상발생
	// timer를 사용하지 않고 직접 time을 받아서 사용하도록 함
	MachineGunDelayCurrentTime += GetWorld()->GetDeltaSeconds();
	if (MachineGunDelayCurrentTime > MachineGunDelay)
	{
		MachineGunDelayCurrentTime = 0;
		Boss->GetLMachineGun()->CreateBullet(Lt,dir);
		Boss->GetRMachineGun()->CreateBullet(Rt,dir);
	}
	
	RotateToTarget(Boss,0.7);

	if (Boss->GetBossAIController()->FacingDot < 0.85)
	{
		Boss->GetBossAnimInstance()->OnPlayMontage(EBossMontage::Turn);
	}
}

void UBossCombatState::DiscLauncher(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using DiscLauncher"));
}

void UBossCombatState::MouseLaser(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("Using MouseLaser"));
}
