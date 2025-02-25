
#include "Boss/ThunderJaw.h"

#include "Project_V.h"
#include "Blueprint/UserWidget.h"
#include "Boss/BossHPUI.h"
#include "Boss/Weapon/MachineGun.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Boss/ThunderJawFSM.h"
#include "Boss/State/BossBaseState.h"
#include "Boss/Weapon/DiscLauncher.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayCharacter.h"
#include "Player/Weapon/Arrow.h"
#include "Player/Weapon/Wire.h"


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

	EyeMatInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);

	//WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPUI"));
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
		splineComp = UGameplayStatics::GetActorOfClass(GetWorld(),SplineClass)->FindComponentByClass<USplineComponent>();
	}

	BossAIController = Cast<AThunderJawAIController>(GetController());
	Aloy = Cast<APlayCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	BossAnimInstance = Cast<UThunderJawAnimInstance>(GetMesh()->GetAnimInstance());
	GetCharacterMovement()->MaxWalkSpeed = BossSpeed;
	ChangeEyeColor(FLinearColor(0,0.14,1),500);
}

UThunderJawFSM* AThunderJaw::GetFSMComponent()
{
	return FSM;
}

AThunderJawAIController* AThunderJaw::GetBossAIController()
{
	return BossAIController;
}

class UThunderJawAnimInstance* AThunderJaw::GetBossAnimInstance()
{
	return BossAnimInstance;
}

class UMaterialInstanceDynamic* AThunderJaw::GetEyeMatInst()
{
	return EyeMatInst;
}

APlayCharacter* AThunderJaw::GetAloy()
{
	return Aloy;
}

class AMachineGun* AThunderJaw::GetLMachineGun()
{
	return LMachineGun;
}

class AMachineGun* AThunderJaw::GetRMachineGun()
{
	return RMachineGun;
}

class ADiscLauncher* AThunderJaw::GetLDiscLauncher()
{
	return LDiscLauncher;
}

class ADiscLauncher* AThunderJaw::GetRDiscLauncher()
{
	return RDiscLauncher;
}



void AThunderJaw::MachineGunBroken(float LeftorRight)
{
	bPartsBroken = true;
	FSM->ChangeBossState(EBossState::Damage);

	if (LeftorRight == -1)
	{
		LMachineGun = nullptr;
	}
	else
	{
		RMachineGun = nullptr;
	}
}

void AThunderJaw::DiscLauncherBroken(float LeftorRight)
{
	bPartsBroken = true;
	FSM->ChangeBossState(EBossState::Damage);

	if (LeftorRight == -1)
	{
		LDiscLauncher = nullptr;
	}
	else
	{
		RDiscLauncher = nullptr;
	}
}

void AThunderJaw::ChangeEyeColor(FLinearColor color, float emissivePower)
{
	if (EyeMatInst)
	{
		EyeMatInst->SetVectorParameterValue(FName("EyeColor"),color);
		EyeMatInst->SetScalarParameterValue(FName("EmissivePower"),emissivePower);
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
	if (bIsLSEnd)
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


void AThunderJaw::DrawDebugCircle(UWorld* World, FVector Center, float Radius)
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

void AThunderJaw::OnBossBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* arrow = Cast<AArrow>(OtherActor);
	if (arrow)
	{
		if (FSM->GetCurrentState()->BossState != EBossState::Combat && FSM->GetCurrentState()->BossState != EBossState::Damage)
		{
			BossAIController->DetectedTarget = true;
			FSM->ChangeBossState(EBossState::Combat);
		}
		CurrentHP -= 100.0f;
		
		PRINTLOG(TEXT("%f"),CurrentHP);
		OtherActor->Destroy();
		return;
	}

	auto* wire = Cast<AWire>(OtherActor);
	if(wire)
	{
		CurrentHP -= 200.0f;
		bTrapped = true;
		FSM->ChangeBossState(EBossState::Damage);
		OtherActor->Destroy();
	}
}








