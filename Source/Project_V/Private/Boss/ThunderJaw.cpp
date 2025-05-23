
#include "Boss/ThunderJaw.h"

#include "EngineUtils.h"
#include "Project_V.h"
#include "Boss/Weapon/MachineGun.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Boss/ThunderJawFSM.h"
#include "Boss/State/BossBaseState.h"
#include "Boss/Weapon/DiscLauncher.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayCharacter.h"
#include "Player/Weapon/Arrow.h"
#include "Player/Weapon/Wire.h"
#include "UI/DamageUI.h"


// Sets default values
AThunderJaw::AThunderJaw()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AThunderJawAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	InitConstruct();
}

void AThunderJaw::BeginPlay()
{
	Super::BeginPlay();
	// controller가 생성자에서 할당되지 않기에 Beginplay에서 casting해줌
	
	InitBeginPlay();
	
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this,&AThunderJaw::OnBossBeginOverlap);
}

void AThunderJaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AThunderJaw::InitConstruct()
{
	
	FSM = CreateDefaultSubobject<UThunderJawFSM>(TEXT("FSM"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("'/Game/Assets/SciFi_Beasts_Pack/SciFi_Beast05/Mesh/SK_SciFi_Beast05_Skin1.SK_SciFi_Beast05_Skin1'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0,0,-100.0));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
		GetMesh()->SetRelativeScale3D(FVector(3.0));
		GetMesh()->SetGenerateOverlapEvents(true);
		GetMesh()->SetCollisionProfileName(TEXT("Enemy"));
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat0(TEXT("'/Game/Assets/SciFi_Beasts_Pack/SciFi_Beast05/Materials/Skin1/Mat_SciFi_Beast05_Armor_Skin1.Mat_SciFi_Beast05_Armor_Skin1'"));
	if (tempMat0.Succeeded())
	{
		GetMesh()->SetMaterial(0,tempMat0.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat1(TEXT("'/Game/Assets/SciFi_Beasts_Pack/SciFi_Beast05/Materials/Skin1/Mat_SciFi_Beast05_Body_Skin1.Mat_SciFi_Beast05_Body_Skin1'"));
	if (tempMat1.Succeeded())
	{
		GetMesh()->SetMaterial(1,tempMat1.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("'/Game/Blueprints/Boss/Animation/ABP_ThunderJaw.ABP_ThunderJaw_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	SoundInstance = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundInstance"));

	ConstructorHelpers::FObjectFinder<USoundWave> tempRageSound(TEXT("'/Game/Blueprints/Boss/Sounds/RageSound.RageSound'"));
	if (tempRageSound.Succeeded())
	{
		RageSound = tempRageSound.Object;
	}
	
	ConstructorHelpers::FObjectFinder<USoundWave> tempWalkSound(TEXT("'/Game/Blueprints/Boss/Sounds/WalkSound.WalkSound'"));
	if (tempWalkSound.Succeeded())
	{
		WalkSound = tempWalkSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave> tempChargeSound(TEXT("'/Game/Blueprints/Boss/Sounds/ChargeSound.ChargeSound'"));
	if (tempChargeSound.Succeeded())
	{
		ChargeSound = tempChargeSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave> tempFallDownSound(TEXT("'/Game/Blueprints/Boss/Sounds/FallDownSound.FallDownSound'"));
	if (tempFallDownSound.Succeeded())
	{
		FallDownSound = tempFallDownSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave> tempDieSound(TEXT("'/Game/Blueprints/Boss/Sounds/DieSound.DieSound'"));
	if (tempDieSound.Succeeded())
	{
		DieSound = tempDieSound.Object;
	}
	
	ConstructorHelpers::FObjectFinder<USoundWave> tempMachineGunSound(TEXT("'/Game/Blueprints/Boss/Sounds/MachineGunSound.MachineGunSound'"));
	if (tempMachineGunSound.Succeeded())
	{
		MachineGunSound = tempMachineGunSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave> tempTailSound(TEXT("'/Game/Blueprints/Boss/Sounds/TailSound.TailSound'"));
	if (tempTailSound.Succeeded())
	{
		TailSound = tempTailSound.Object;
	}
	
	ConstructorHelpers::FClassFinder<UUserWidget> tempFloatingText(TEXT("'/Game/Blueprints/UI/WBP_DamageUI.WBP_DamageUI_C'"));
	if (tempFloatingText.Succeeded())
	{
		FloatingTextFactory = tempFloatingText.Class;
	}

	ConstructorHelpers::FClassFinder<AActor> tempRadarEffect(TEXT("'/Game/Blueprints/Boss/BP_RadarEffect.BP_RadarEffect_C'"));
	if (tempRadarEffect.Succeeded())
	{
		RadarEffectFactory = tempRadarEffect.Class;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempHitSound(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Robot_Hit.Robot_Hit'"));
	if (tempHitSound.Succeeded())
	{
		HitSound = tempHitSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempDestructSound(TEXT("/Script/Engine.SoundWave'/Game/Sounds/robot_fall.robot_fall'"));
	if (tempDestructSound.Succeeded())
	{
		DestructSound = tempDestructSound.Object;
	}

	ConstructorHelpers::FClassFinder<UCameraShakeBase> tempCameraShake(TEXT("/Script/Engine.Blueprint'/Game/Assets/CinematicFX/BP/BP_CameraShake.BP_CameraShake_C'"));
	if (tempCameraShake.Succeeded())
	{
		CameraShakeFactory = tempCameraShake.Class;
	}
	
	EyeMatInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
}

void AThunderJaw::InitBeginPlay()
{
	UMaterialInterface* BaseMat = GetMesh()->GetMaterial(1);
	if (BaseMat)
	{
		EyeMatInst = UMaterialInstanceDynamic::Create(BaseMat,this);
		GetMesh()->SetMaterial(1,EyeMatInst);
	}
	
	LMachineGun = GetWorld()->SpawnActor<AMachineGun>(AMachineGun::StaticClass());
	if (LMachineGun)
	{
		LMachineGun->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("LMachineGunSocket"));
		LMachineGun->Root->SetCollisionProfileName("Enemy");
		LMachineGun->LeftorRight = -1;
		LMachineGun->Boss = this;
	}
	
	RMachineGun = GetWorld()->SpawnActor<AMachineGun>(AMachineGun::StaticClass());
	if (RMachineGun)
	{
		RMachineGun->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("RMachineGunSocket"));
		RMachineGun->Root->SetCollisionProfileName("Enemy");
		RMachineGun->LeftorRight = 1;
		RMachineGun->Boss = this;

	}

	LDiscLauncher = GetWorld()->SpawnActor<ADiscLauncher>(ADiscLauncher::StaticClass());
	if (LDiscLauncher)
	{
		LDiscLauncher->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LDiscLauncherSocket"));
		LDiscLauncher->Root->SetCollisionProfileName("Enemy");
		LDiscLauncher->LeftorRight = -1;
		LDiscLauncher->Boss = this;
	}
	
	RDiscLauncher = GetWorld()->SpawnActor<ADiscLauncher>(ADiscLauncher::StaticClass());
	if (RDiscLauncher)
	{
		RDiscLauncher->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RDiscLauncherSocket"));
		RDiscLauncher->Root->SetCollisionProfileName("Enemy");
		RDiscLauncher->LeftorRight = 1;
		RDiscLauncher->Boss = this;
	}

	TSubclassOf<AActor> SplineClass = LoadClass<AActor>(nullptr,TEXT("'/Game/Blueprints/Boss/BP_Spline.BP_Spline_C'"));
	if (SplineClass)
	{
		SplineComp = UGameplayStatics::GetActorOfClass(GetWorld(),SplineClass)->FindComponentByClass<USplineComponent>();
	}

	// TActorIterator를 사용하여 특정 클래스의 액터 검색
	for (TActorIterator<AActor> It(GetWorld(), AActor::StaticClass()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->IsA(AActor::StaticClass()) && Actor->GetName().Contains(TEXT("BP_Spline")))
		{
			SplineBP = Actor;
			UE_LOG(LogTemp, Log, TEXT("Found Spline Actor: %s"), *SplineBP->GetName());
			break; // 첫 번째 찾은 액터 저장 후 루프 종료
		}
	}
	SplineBP->SetHidden(true);

	WidgetComp = GetComponentByClass<UWidgetComponent>();
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(false);
	}

	BossAIController = Cast<AThunderJawAIController>(GetController());
	Aloy = Cast<APlayCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	BossAnimInstance = Cast<UThunderJawAnimInstance>(GetMesh()->GetAnimInstance());
	GetCharacterMovement()->MaxWalkSpeed = BossSpeed;
	ChangeEyeColor(FLinearColor(0,0.14,1),500);

	FloatingDamage.AddDynamic(this,&AThunderJaw::SpawnDamageUI);
}

UThunderJawFSM* AThunderJaw::GetFSMComponent() const
{
	return FSM;
}

AThunderJawAIController* AThunderJaw::GetBossAIController() const
{
	return BossAIController;
}

class UThunderJawAnimInstance* AThunderJaw::GetBossAnimInstance() const
{
	return BossAnimInstance;
}

class UMaterialInstanceDynamic* AThunderJaw::GetEyeMatInst() const
{
	return EyeMatInst;
}

APlayCharacter* AThunderJaw::GetAloy() const
{
	return Aloy;
}

class AMachineGun* AThunderJaw::GetLMachineGun() const
{
	return LMachineGun;
}

class AMachineGun* AThunderJaw::GetRMachineGun() const
{
	return RMachineGun;
}

class ADiscLauncher* AThunderJaw::GetLDiscLauncher() const
{
	return LDiscLauncher;
}

class ADiscLauncher* AThunderJaw::GetRDiscLauncher() const
{
	return RDiscLauncher;
}

class UWidgetComponent* AThunderJaw::GetWidgetComponent() const
{
	return WidgetComp;
}

class USplineComponent* AThunderJaw::GetSplineComponent() const
{
	return SplineComp;
}

TSubclassOf<AActor> AThunderJaw::GetRadarEffectFactory() const
{
	return RadarEffectFactory;
}

void AThunderJaw::MachineGunBroken(float LeftOrRight)
{
	bPartBroken = true;
	FSM->ChangeBossState(EBossState::Damage);

	if (LeftOrRight == -1)
	{
		LMachineGun = nullptr;
	}
	else
	{
		RMachineGun = nullptr;
	}
}

void AThunderJaw::DiscLauncherBroken(float LeftOrRight)
{
	bPartBroken = true;
	FSM->ChangeBossState(EBossState::Damage);

	if (LeftOrRight == -1)
	{
		LDiscLauncher = nullptr;
	}
	else
	{
		RDiscLauncher = nullptr;
	}
}

void AThunderJaw::ChangeEyeColor(FLinearColor Color, float EmissivePower)
{
	if (EyeMatInst)
	{
		EyeMatInst->SetVectorParameterValue(FName("EyeColor"),Color);
		EyeMatInst->SetScalarParameterValue(FName("EmissivePower"),EmissivePower);
	}
}

void AThunderJaw::RotateToTarget(FVector TargetLoc, float InterpSpeed)
{
	// 타겟 위치로 몸을 돌리는 함수
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLoc);
	float Delta = LookAtRot.Yaw - UKismetMathLibrary::NormalizeAxis(GetActorRotation().Yaw);
	Delta = UKismetMathLibrary::NormalizeAxis(Delta);

	float NewYaw = UKismetMathLibrary::NormalizeAxis(GetActorRotation().Yaw) + Delta;
	
	float NewDestYaw = UKismetMathLibrary::FInterpTo(UKismetMathLibrary::NormalizeAxis(GetActorRotation().Yaw),NewYaw, GetWorld()->GetDeltaSeconds(),InterpSpeed);
	
	SetActorRotation(FRotator(0,NewDestYaw,0));
}

void AThunderJaw::SetVisibilityBoss()
{
	if (bIsLevelSequenceEnd)
	{
		SetActorHiddenInGame(false);
		LMachineGun->SetActorHiddenInGame(false);
		RMachineGun->SetActorHiddenInGame(false);
		LDiscLauncher->SetActorHiddenInGame(false);
		RDiscLauncher->SetActorHiddenInGame(false);
	}
	else
	{
		SetActorHiddenInGame(true);
		LMachineGun->SetActorHiddenInGame(true);
		RMachineGun->SetActorHiddenInGame(true);
		LDiscLauncher->SetActorHiddenInGame(true);
		RDiscLauncher->SetActorHiddenInGame(true);
	}
}


void AThunderJaw::DrawDebugCircle(const UWorld* World, const FVector& Center, const float Radius)
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

void AThunderJaw::ChangeToFocusModeMat(const bool bFocusMode)
{
	LMachineGun->CheckFocusModeAndChangeMat(bFocusMode);
	RMachineGun->CheckFocusModeAndChangeMat(bFocusMode);
	LDiscLauncher->CheckFocusModeAndChangeMat(bFocusMode);
	RDiscLauncher->CheckFocusModeAndChangeMat(bFocusMode);
}

void AThunderJaw::SetVisibilitySpline(const bool bFocusMode)
{
	if (bFocusMode)
	{
		SplineBP->SetHidden(false);
	}
	else
	{
		SplineBP->SetHidden(true);
	}
}

void AThunderJaw::BossTakeDamage(int Damage)
{
	CurrentHP -= Damage;
	if (CurrentHP <= 0)
	{
		FSM->ChangeBossState(EBossState::Die);
		GameClear();
	}
	FloatingDamage.Broadcast(Damage);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());
}

void AThunderJaw::SpawnDamageUI(const float Damage)
{
	DamageUI = Cast<UDamageUI>(CreateWidget(GetWorld(),FloatingTextFactory));
	if (DamageUI)
	{
		DamageUI->SetDamageText(Damage);
		DamageUI->PlayAnimAndRemoveDamageUI();
		DamageUI->AddToViewport();
	}
}

void AThunderJaw::GameClear()
{
	Aloy->ShowGameStateUI.Broadcast(true);
}

void AThunderJaw::OnBossBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* arrow = Cast<AArrow>(OtherActor);
	if (arrow && arrow->hitCollision->IsCollisionEnabled())
	{
		if (FSM->GetCurrentState()->BossState != EBossState::Combat && FSM->GetCurrentState()->BossState != EBossState::Damage)
		{
			BossAIController->DetectedTarget = true;
			FSM->ChangeBossState(EBossState::Combat);
		}

		BossTakeDamage(100);
		
		PRINTLOG(TEXT("Hit Arrow, HP : %f"),CurrentHP);
		arrow->DestroyAfterPlayFX();
	}

	auto* wire = Cast<AWire>(OtherActor);
	if(wire)
	{
		if (wire->cableCollision->IsCollisionEnabled())
		{
			bTrapped = true;
			FSM->ChangeBossState(EBossState::Damage);

			BossTakeDamage(300);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DestructSound, GetActorLocation());
			ShakeCamera();
		
			PRINTLOG(TEXT("Hit Wire, HP : %f"),CurrentHP);
			wire->DestroyAfterPlayFX();
		}
	}
}

void AThunderJaw::ShakeCamera()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeFactory, GetActorLocation(), 200, 10000);
}








