// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/MachineGun.h"

#include "Project_V.h"
#include "Boss/Weapon/MachineGunBullet.h"
#include "Components/BoxComponent.h"
#include "Player/Arrow.h"


// Sets default values
AMachineGun::AMachineGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitComponents();
}

// Called when the game starts or when spawned
void AMachineGun::BeginPlay()
{
	Super::BeginPlay();

	MaxHP = 100.0f;
	CurrentHP = MaxHP;
	Root->OnComponentBeginOverlap.AddDynamic(this,&AMachineGun::OnMachineGunOverlap);
}

// Called every frame
void AMachineGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsBroken)
	{
		Root->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
		PRINTLOG(TEXT("detached"));
		UPrimitiveComponent* primComp = GetComponentByClass<UPrimitiveComponent>();
		if (primComp)
		{
			primComp->SetSimulatePhysics(true);
			primComp->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
		}
	}
}

void AMachineGun::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
		Root->SetRelativeScale3D(FVector(3.0,3.0,5.0));
		Root->SetBoxExtent(FVector(18.0,20.0,12.0));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeScale3D(FVector(0.3,0.1,0.2));
	}

	FirePos = CreateDefaultSubobject<USceneComponent>(TEXT("FirePos"));
	if (FirePos)
	{
		FirePos->SetupAttachment(Root);
		FirePos->SetRelativeLocation(FVector(30,0,0));
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0,tempMat.Object);
	}

	ConstructorHelpers::FClassFinder<AMachineGunBullet> tempBullet(TEXT("'/Game/Blueprints/Boss/BP_MachineGunBullet.BP_MachineGunBullet_C'"));
	if (tempBullet.Succeeded())
	{
		BulletFactory = tempBullet.Class;
	}
}

void AMachineGun::CreateBullet(FTransform transform, FVector direction)
{
	AMachineGunBullet* bullet = GetWorld()->SpawnActor<AMachineGunBullet>(BulletFactory,transform);
	if (bullet)
	{
		bullet->FireInDirection(direction);
	}
}

void AMachineGun::OnMachineGunOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* arrow = Cast<AArrow>(OtherActor);
	if (arrow)
	{
		DamageWeaponHP(50);
		PRINTLOG(TEXT("%s hit, hp : %f"),*this->GetName(), this->CurrentHP);
		arrow->Destroy();
	}
}
