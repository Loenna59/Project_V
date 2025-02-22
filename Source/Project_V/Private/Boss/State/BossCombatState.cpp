// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/State/BossCombatState.h"

#include "Project_V.h"
#include "Boss/Weapon/MachineGun.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayCharacter.h"


void UBossCombatState::Enter(AThunderJaw* Boss, UThunderJawFSM* FSM)
{
	Super::Enter(Boss, FSM);
	Boss->ChangeEyeColor(FLinearColor(1,0,0),2000);
}

void UBossCombatState::Update(AThunderJaw* Boss, UThunderJawFSM* FSM, float DeltaTime)
{
	Super::Update(Boss, FSM, DeltaTime);

	if (!Boss->GetAloy())
	{
		return;
	}
	
	// 몸을 돌리는 중이면 공격하지 않음
	if (bIsRotateBody)
	{
		Boss->RotateToTarget(Boss->GetAloy()->GetActorLocation(),1.0);
		Boss->GetBossAnimInstance()->OnPlayTurnMontage();
		
		// enemy의 정면까지 돌렸으면 false로 변경
		if (Boss->GetBossAIController()->FacingDot > 0.8)
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
				Attack(Boss);
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

void UBossCombatState::InitComponents(AThunderJaw* Boss)
{
	UsingPattern = EAttackPattern::None;
	PatternCurrentTime = 0;
	PatternTime = 0;
	ChargeFlag = false;
	Boss->GetCharacterMovement()->MaxWalkSpeed = Boss->BossSpeed;
}

void UBossCombatState::MakeTraceBoxAndCheckHit(FVector start, FVector end, FVector boxHalfSize)
{
	FCollisionQueryParams Params;
	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		start,
		end,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeBox(boxHalfSize),
		Params);

	FColor BoxColor = FColor::Green;
	if (bHit)
	{
		for (const FHitResult hit : HitResults)
		{
			auto* player = Cast<APlayCharacter>(hit.GetActor());
			if (player)
			{
				PRINTLOG(TEXT("player hit"));
				BoxColor = FColor::Red;
			}
		}
	}
	else
	{
		BoxColor = FColor::Green;
	}
	
	// 박스 디버깅
	int NumSteps = 10;
	for (int i = 0; i <= NumSteps; i++)
	{
		float Alpha = (float)i / NumSteps;
		FVector DebugLocation = FMath::Lerp(start, end, Alpha);

		DrawDebugBox(GetWorld(), DebugLocation, boxHalfSize, FQuat::Identity, BoxColor, false, 0.1f);
	}
}

void UBossCombatState::Attack(AThunderJaw* Boss)
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
	PRINTLOG(TEXT("Start Pattern"));
	if (GetWorld()->GetTimerManager().IsTimerActive(PatternTimerHandle))
	{
		PRINTLOG(TEXT("Timer Exist"));
		GetWorld()->GetTimerManager().ClearTimer(PatternTimerHandle);
	}

	bIsDelay = true;
	InitComponents(Boss);

	TWeakObjectPtr<AThunderJaw> WeakBoss = Boss;
	auto callBack = [this,WeakBoss]()
	{
		if (WeakBoss.IsValid())
		{
			DelayEndBeforeChoosingPattern(WeakBoss.Get());
		}
	};
	GetWorld()->GetTimerManager().SetTimer(PatternTimerHandle,
		FTimerDelegate::CreateLambda(callBack),PatternDelay,false);
}

void UBossCombatState::DelayEndBeforeChoosingPattern(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("PatternDelayEnd"));
	bIsDelay = false;
	ChooseRandomPattern(Boss);
}

void UBossCombatState::ChooseRandomPattern(AThunderJaw* Boss)
{
	PRINTLOG(TEXT("ChooseRandomPattern"));
	float Dist = Boss->GetBossAIController()->DistanceFromTarget;

	if (Dist <= Boss->MeleeAttackDist)
	{
		int32 randomNum = FMath::RandRange(0,1);
		if (randomNum == 0)
		{
			PRINTLOG(TEXT("Using Charge"));
			UsingPattern = EAttackPattern::Charge;
			PatternTime = recoilTime + ChargeTime;
		}
		else if (randomNum == 1)
		{
			PRINTLOG(TEXT("Using Tail"));
			UsingPattern = EAttackPattern::Tail;
			PatternTime = TailPatternTime;
		}
	}
	else
	{
		int32 randomNum = FMath::RandRange(2,2);
		if (randomNum == 2)
		{
			PRINTLOG(TEXT("Using MachineGun"));
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
		
		GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle,[this, WeakBoss]()
		{
			if (WeakBoss.IsValid())
			{
				ChargeStart = true;
				WeakBoss.Get()->GetCharacterMovement()->MaxWalkSpeed *= 1.75;
			}
		},recoilTime,false);
	}
	
	//PRINTLOG(TEXT("Using Charge"));

	if (!ChargeStart)
	{
		// 플레이어를 바라보면서 뒷걸음질을 하기 위해 움직이는 방향으로 회전하지 않게 막음
		Boss->GetCharacterMovement()->bOrientRotationToMovement = false;
		// 플레이어를 바라보게 몸을 돌려줌
		Boss->RotateToTarget(Boss->GetAloy()->GetActorLocation(),1.0f);
		Boss->AddMovementInput(-PerposeLocation);
	}
	else
	{
		// 돌진 전에 한 설정 되돌리기
		Boss->GetCharacterMovement()->bOrientRotationToMovement = true;
		Boss->AddMovementInput(PerposeLocation);

		FVector HeadStart = Boss->GetMesh()->GetSocketLocation(TEXT("head1"));
		FVector HeadEnd = Boss->GetMesh()->GetSocketLocation(TEXT("head1EndSocket"));
		FVector BoxHalfSize(150,150,100);
		MakeTraceBoxAndCheckHit(HeadStart,HeadEnd,BoxHalfSize);
		
		Boss->GetBossAnimInstance()->OnPlayChargeMontage();

		
	}
}

void UBossCombatState::Tail(AThunderJaw* Boss)
{
	//PRINTLOG(TEXT("Using Tail"));
	Boss->RotateToTarget(Boss->GetAloy()->GetActorLocation(),1.0f);

	FVector TailStart = Boss->GetMesh()->GetSocketLocation(TEXT("tail"));
	FVector TailEnd = Boss->GetMesh()->GetSocketLocation(TEXT("tail6"));
	FVector BoxHalfSize = FVector(300,300,200);
	MakeTraceBoxAndCheckHit(TailStart,TailEnd,BoxHalfSize);
	Boss->GetBossAnimInstance()->OnPlayTailMontage();
}

void UBossCombatState::MachineGun(AThunderJaw* Boss)
{
	// 임시방편 코드
	// 머신건이 둘 다 부숴졌으면 return
	// TODO
	// 머신건이 전부 부숴졌으면 패턴 목록에서 지워라
	if (!Boss->GetLMachineGun() && !Boss->GetRMachineGun())
	{
		PatternTime = 0;
		return;
	}
	
	//PRINTLOG(TEXT("Using Machine Gun"));
	DrawDebugCircle(GetWorld(),Boss->GetAloy()->GetActorLocation(),300.0f);

	// 회전하면서 쏠 때 timer에 loop로 처리하면 위치값이 업데이트 안되는 현상발생
	// timer를 사용하지 않고 직접 time을 받아서 사용하도록 함
	MachineGunDelayCurrentTime += GetWorld()->GetDeltaSeconds();
	if (MachineGunDelayCurrentTime > MachineGunDelay)
	{
		MachineGunDelayCurrentTime = 0;
		if (Boss->GetLMachineGun())
		{
			FTransform Lt = Boss->GetLMachineGun()->FirePos->GetComponentTransform();
			Lt.SetScale3D(FVector(1.0));
			Boss->GetLMachineGun()->CreateBullet(Lt,Boss->GetAloy()->GetActorLocation());
		}
		if (Boss->GetRMachineGun())
		{
			FTransform Rt = Boss->GetRMachineGun()->FirePos->GetComponentTransform();
			Rt.SetScale3D(FVector(1.0));
			Boss->GetRMachineGun()->CreateBullet(Rt,Boss->GetAloy()->GetActorLocation());
		}
	}
	
	Boss->RotateToTarget(Boss->GetAloy()->GetActorLocation(),1.0);

	if (Boss->GetBossAIController()->FacingDot < 0.75)
	{
		Boss->GetBossAnimInstance()->OnPlayTurnMontage();
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

void UBossCombatState::DrawDebugCircle(UWorld* World, FVector Center, float Radius)
{
	if (!World) return;

	const float AngleStep = 2.0f * PI / 32;
	FVector PrevPoint = Center + FVector(Radius, 0.0f, 0.0f);
    
	for (int32 i = 1; i <= 32; i++)
	{
		float Angle = AngleStep * i;
		FVector NextPoint = Center + FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
        
		DrawDebugLine(
			World,
			PrevPoint,
			NextPoint,
			FColor::Red,
			false,
			-1.0f,
			0,
			1
		);
        
		PrevPoint = NextPoint;
	}
}
