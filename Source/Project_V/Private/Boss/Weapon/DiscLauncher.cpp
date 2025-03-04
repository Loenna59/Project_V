// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/DiscLauncher.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawFSM.h"
#include "Boss/Weapon/Disc.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Player/Weapon/Arrow.h"


// Sets default values
ADiscLauncher::ADiscLauncher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitComponents();
}


// Called when the game starts or when spawned
void ADiscLauncher::BeginPlay()
{
	Super::BeginPlay();

	MaxHP = 200.0f;
	CurrentHP = MaxHP;
	Root->OnComponentBeginOverlap.AddDynamic(this,&ADiscLauncher::OnDiscLauncherOverlap);
}

void ADiscLauncher::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
		Root->SetBoxExtent(FVector(150,80,80));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeScale3D(FVector(2.5,1.0,1.0));
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("'/Engine/MapTemplates/Materials/BasicAsset02.BasicAsset02'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0,tempMat.Object);
	}

	FirePos = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePos"));
	if (FirePos)
	{
		FirePos->SetupAttachment(Root);
		FirePos->SetRelativeLocation(FVector(160,0,0));
	}

	ConstructorHelpers::FClassFinder<ADisc> tempDisc(TEXT("'/Game/Blueprints/Boss/BP_Disc.BP_Disc_C'"));
	if (tempDisc.Succeeded())
	{
		DiscFactory = tempDisc.Class;	
	}
		
}

void ADiscLauncher::CreateDisc(FTransform transfrom)
{
	GetWorld()->SpawnActor<ADisc>(DiscFactory,transfrom);
}

void ADiscLauncher::OnDiscLauncherOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PRINTLOG(TEXT("%s"),*OtherActor->GetName());
	auto* arrow = Cast<AArrow>(OtherActor);
	if (arrow && arrow->hitCollision->IsCollisionEnabled())
	{
		DamageWeaponHP(100);
		Boss->BossTakeDamage(150);
		PRINTLOG(TEXT("%s hit, hp : %f"),*this->GetName(), this->CurrentHP);
	
		// 경계중이거나 정찰중일 때 맞으면 combat state로 전환
		Boss->GetBossAIController()->DetectedTarget = true;
		Boss->GetFSMComponent()->ChangeBossState(EBossState::Combat);
		
		// detach from parent
		if (bIsBroken)
		{
			Boss->DiscLauncherBroken(LeftorRight);
			Root->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
			Root->SetCollisionProfileName(FName("BlockAll"));
			UPrimitiveComponent* primComp = GetComponentByClass<UPrimitiveComponent>();
			if (primComp)
			{
				primComp->SetSimulatePhysics(true);
				primComp->SetAllMassScale(500.0f);
			}
		}
		arrow->Destroy();
	}
}



