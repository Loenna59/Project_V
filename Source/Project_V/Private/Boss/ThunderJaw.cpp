
#include "Boss/ThunderJaw.h"

#include "NavigationInvokerComponent.h"
#include "Boss/Weapon/MachineGun.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Boss/ThunderJawFSM.h"
#include "Boss/Weapon/DiscLauncher.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayCharacter.h"


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
	float NewYaw = UKismetMathLibrary::FInterpTo(GetActorRotation().Yaw,LookAtRot.Yaw, GetWorld()->GetDeltaSeconds(),InterpSpeed);
	SetActorRotation(FRotator(0,NewYaw,0));
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









