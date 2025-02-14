
#include "Boss/ThunderJaw.h"

#include "Project_V.h"
#include "Boss/MachineGun.h"
#include "Boss/MachineGunBullet.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawAnimInstance.h"
#include "Boss/ThunderJawFSM.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayCharacter.h"


// Sets default values
AThunderJaw::AThunderJaw()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AThunderJawAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	InitComponents();
}

void AThunderJaw::BeginPlay()
{
	Super::BeginPlay();
	// controller가 생성자에서 할당되지 않기에 Beginplay에서 casting해줌
	BossAIController = Cast<AThunderJawAIController>(GetController());
	if (!BossAIController)
	{
		PRINTLOG(TEXT("BossAIController Cast Failed"));
	}
	Aloy = Cast<APlayCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	LMachineGun = GetWorld()->SpawnActor<AMachineGun>(AMachineGun::StaticClass());
	if (LMachineGun)
	{
		LMachineGun->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("LMachineGunSocket"));
	}
	
	RMachineGun = GetWorld()->SpawnActor<AMachineGun>(AMachineGun::StaticClass());
	if (RMachineGun)
	{
		RMachineGun->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("RMachineGunSocket"));
	}

	BossAnimInstance = Cast<UThunderJawAnimInstance>(GetMesh()->GetAnimInstance());
}

void AThunderJaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThunderJaw::InitComponents()
{
	
	FSM = CreateDefaultSubobject<UThunderJawFSM>(TEXT("FSM"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("'/Game/Assets/SciFi_Beasts_Pack/SciFi_Beast05/Mesh/SK_SciFi_Beast05_Skin1.SK_SciFi_Beast05_Skin1'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0,0,-70.0f));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("'/Game/Blueprints/Boss/Animation/ABP_ThunderJaw.ABP_ThunderJaw_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}
	
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
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










