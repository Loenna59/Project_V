// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/MachineGunBullet.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Boss/Weapon/MachineGun.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayCharacter.h"


// Sets default values
AMachineGunBullet::AMachineGunBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitComponents();
}

// Called when the game starts or when spawned
void AMachineGunBullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle deathTimer;
	GetWorldTimerManager().SetTimer(deathTimer, [this]()
	{
		BackToMagazine();
	}, LifeTime, false);


	Root->OnComponentBeginOverlap.AddDynamic(this,&AMachineGunBullet::OnBulletBeginOverlap);
}

// Called every frame
void AMachineGunBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMachineGunBullet::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
		Root->SetBoxExtent(FVector(30,10,10));
		Root->SetGenerateOverlapEvents(true);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeLocation(FVector(-30,0,0));
		Mesh->SetRelativeRotation(FRotator(-90,0,0));
		Mesh->SetRelativeScale3D(FVector(0.1,0.1,0.6));
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("'/Game/Blueprints/Boss/Material/M_MachineGunBullet.M_MachineGunBullet'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0,tempMat.Object);
	}
	
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMC"));
	if (PMC)
	{
		PMC->SetUpdatedComponent(Root);
		PMC->InitialSpeed = 5000.0f;
		PMC->MaxSpeed = 5000.0f;
		PMC->bRotationFollowsVelocity = true;
		PMC->bShouldBounce = true;
		PMC->Bounciness = 0.3f;
		PMC->ProjectileGravityScale = 1.0f;
		PMC->bAutoActivate = false;
	}

	ConstructorHelpers::FObjectFinder<USoundWave> tempGunSound(TEXT("'/Game/Blueprints/Boss/Sounds/GunSound.GunSound'"));
	if (tempGunSound.Succeeded())
	{
		GunSound = tempGunSound.Object;
	}
}

void AMachineGunBullet::OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	auto* player = Cast<APlayCharacter>(OtherActor);
	if (player)
	{
		player->HitDamage(10.f, GetActorForwardVector());
	}
	BackToMagazine();

}

FVector2D AMachineGunBullet::GetRandomPointInCircle(float radius, FVector2D centerPoint)
{
	float RandomAngle = FMath::RandRange(0.0f,2.0f*PI);
	float RandomRadius = radius * FMath::Sqrt(FMath::FRand());

	float X = centerPoint.X + RandomRadius * FMath::Cos(RandomAngle);
	float Y = centerPoint.Y + RandomRadius * FMath::Sin(RandomAngle);

	return FVector2D(X,Y);
}

FVector AMachineGunBullet::GetRandomPointInCircleXY(float radius, FVector centerPoint)
{
	FVector2D random2D = GetRandomPointInCircle(radius,FVector2D(centerPoint.X,centerPoint.Y));
	return FVector(random2D.X,random2D.Y,centerPoint.Z);
}

void AMachineGunBullet::SetBulletActive(bool value) const
{
	if (value)
	{
		//PRINTLOG(TEXT("call bullet"));
		PMC->SetActive(true);
		//Root->SetCollisionProfileName("EnemyProjectile");
		Mesh->SetVisibility(true);
	}
	else
	{
		PMC->SetActive(false);
		//Root->SetCollisionProfileName("NoCollision");
		Mesh->SetVisibility(false);
	}
}

void AMachineGunBullet::FireToTarget(const FVector& Target, float Radius)
{
	FVector randomPoint = GetRandomPointInCircleXY(Radius,Target);
	FVector randomDir = (randomPoint - GetActorLocation()).GetSafeNormal();
	PMC->Velocity = randomDir * PMC->InitialSpeed;
}

void AMachineGunBullet::BackToMagazine()
{
	SetBulletActive(false);
	if (MachineGun)
	{
		SetActorLocation(FVector(10000));
		MachineGun->Magazine.Add(this);
	}
}
