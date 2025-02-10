
#include "Boss/ThunderJaw.h"

#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawFSM.h"
#include "Player/PlayCharacter.h"


// Sets default values
AThunderJaw::AThunderJaw()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AThunderJawAIController::StaticClass();
	InitComponents();
}

void AThunderJaw::BeginPlay()
{
	Super::BeginPlay();
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

	PatrolPoints.Add(FVector(965.000000,1310.000000,0.000000));
	PatrolPoints.Add(FVector(2335.000000,1315.000000,0.000000));
}








