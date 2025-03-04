// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/MachineGun.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Boss/ThunderJawFSM.h"
#include "Boss/Weapon/MachineGunBullet.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Player/Weapon/Arrow.h"


// Sets default values
AMachineGun::AMachineGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitComponents();
}

// Called when the game starts or when spawned
void AMachineGun::BeginPlay()
{
	Super::BeginPlay();

	
	for (int32 i = 0; i < BulletMaxCount; i++)
	{
		auto* bullet = GetWorld()->SpawnActor<AMachineGunBullet>(BulletFactory);
		bullet->SetActorLocation(FVector(10000));
		bullet->SetBulletActive(false);
		Boss->Magazine.Add(bullet);
	}

	MaxHP = 200.0f;
	CurrentHP = MaxHP;
	Root->OnComponentBeginOverlap.AddDynamic(this,&AMachineGun::OnMachineGunOverlap);
}

void AMachineGun::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
		Root->SetRelativeScale3D(FVector(3.0,3.0,5.0));
		Root->SetBoxExtent(FVector(25.0,20.0,15.0));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeLocation(FVector(14.701525,0.000000,-6.119322));
		Mesh->SetRelativeScale3D(FVector(2.5));
		Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}

	FirePos = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePos"));
	if (FirePos)
	{
		FirePos->SetupAttachment(Root);
		FirePos->SetRelativeLocation(FVector(30,0,0));
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("'/Game/Fab/Hand_RailGun/hand_railgun.hand_railgun'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat0(TEXT("'/Game/Fab/Hand_RailGun/HandRailGun.HandRailGun'"));
	if (tempMat0.Succeeded())
	{
		Mesh->SetMaterial(0,tempMat0.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat1(TEXT("'/Game/Fab/Hand_RailGun/HandRailGun_glass.HandRailGun_glass'"));
	if (tempMat1.Succeeded())
	{
		Mesh->SetMaterial(1,tempMat1.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat2(TEXT("'/Game/Fab/Hand_RailGun/HandRailGun_screen.HandRailGun_screen'"));
	if (tempMat2.Succeeded())
	{
		Mesh->SetMaterial(2,tempMat2.Object);
	}

	ConstructorHelpers::FClassFinder<AMachineGunBullet> tempBullet(TEXT("'/Game/Blueprints/Boss/BP_MachineGunBullet.BP_MachineGunBullet_C'"));
	if (tempBullet.Succeeded())
	{
		BulletFactory = tempBullet.Class;
	}
}

void AMachineGun::CreateBullet(FTransform transform, FVector Target)
{
	// AMachineGunBullet* bullet = GetWorld()->SpawnActor<AMachineGunBullet>(BulletFactory,transform);
	// if (bullet)
	// {
	// 	bullet->FireToTarget(Target, RandomSprayRadius);
	// }

	if (Boss->Magazine.Num() > 0)
	{
		auto* bullet = Boss->Magazine[0];
		bullet->SetBulletActive(true);
		bullet->SetActorTransform(transform);
		bullet->FireToTarget(Target,RandomSprayRadius);
		bullet->PlayGunSound();
		Boss->Magazine.RemoveAt(0);
	}
}

void AMachineGun::OnMachineGunOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PRINTLOG(TEXT("%s"),*OtherActor->GetName());
	auto* arrow = Cast<AArrow>(OtherActor);
	if (arrow && arrow->hitCollision->IsCollisionEnabled())
	{
		DamageWeaponHP(100);
		Boss->BossTakeDamage(150);

		PRINTLOG(TEXT("%s hit, hp : %f"),*this->GetName(), this->CurrentHP);
		Boss->GetBossAIController()->DetectedTarget = true;
		Boss->GetFSMComponent()->ChangeBossState(EBossState::Combat);
		// Detach from parent
		if (bIsBroken)
		{
			Boss->MachineGunBroken(LeftorRight);
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
