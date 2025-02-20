// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/MachineGunBullet.h"

#include "Project_V.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	Root->OnComponentBeginOverlap.AddDynamic(this,&AMachineGunBullet::OnBulletBeginOverlap);

	FTimerHandle deathTimer;
	auto dieCallBack = [this]()->void{Destroy();};
	GetWorldTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda(dieCallBack), LifeTime, false);
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
		Root->SetGenerateOverlapEvents(true);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeLocation(FVector(-30,0,0));
		Mesh->SetRelativeRotation(FRotator(0,-90,0));
		Mesh->SetRelativeScale3D(FVector(0.15,0.15,0.75));
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
	}
}

void AMachineGunBullet::OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* player = Cast<APlayCharacter>(OtherActor);
	if (player)
	{
		Destroy();
	}
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

void AMachineGunBullet::FireToTarget(const FVector& Target, float Radius)
{
	FVector randomPoint = GetRandomPointInCircleXY(Radius,Target);
	FVector randomDir = (randomPoint - GetActorLocation()).GetSafeNormal();
	PMC->Velocity = randomDir * PMC->InitialSpeed;
}