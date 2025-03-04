// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/Disc.h"

#include "Project_V.h"
#include "Boss/Weapon/DiscMissile.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnitConversion.h"


// Sets default values
ADisc::ADisc()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitComponents();
}


// Called when the game starts or when spawned
void ADisc::BeginPlay()
{
	Super::BeginPlay();
	FVector centerPoint = GetActorLocation() + (GetActorForwardVector() * 3000.0f);
	PerposeLocation = GetRandomPointInCircleXY(1000.0f,centerPoint);
}

// Called every frame
void ADisc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToPerposeLocation();
}

void ADisc::InitComponents()
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
		Mesh->SetRelativeScale3D(FVector(0.05));
		Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("'/Game/Fab/UFO_Doodle/scene.scene'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("'/Game/Fab/UFO_Doodle/Ovni.Ovni'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0,tempMat.Object);
	}

	FirePos = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePos"));
	if (FirePos)
	{
		FirePos->SetupAttachment(Root);
		FirePos->SetRelativeLocation(FVector(50,0,0));
	}

	ConstructorHelpers::FClassFinder<AActor> tempTrail(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Boss/BP_LockOnTrail.BP_LockOnTrail_C'"));
	if (tempTrail.Succeeded())
	{
		TrailFactory = tempTrail.Class;
	}

	ConstructorHelpers::FClassFinder<ADiscMissile> tempMissile(TEXT("'/Game/Blueprints/Boss/BP_DiscMissile.BP_DiscMissile_C'"));
	if (tempMissile.Succeeded())
	{
		MissileFactory = tempMissile.Class;
	}
}

void ADisc::MoveToPerposeLocation()
{
	if (bIsArrive)
	{
		return;
	}

	
	float dist = FVector::Distance(PerposeLocation,GetActorLocation());
	if (dist <= AcceptanceRadius)
	{
		bIsArrive = true;
		TargetLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		LaunchMissileToTarget();
		return;
	}
	
	FVector p0 = GetActorLocation();
	FVector direction = (PerposeLocation - GetActorLocation()).GetSafeNormal();
	FVector vt = direction * Speed * GetWorld()->GetDeltaSeconds();
	SetActorLocation(p0 + vt);
}

void ADisc::LaunchMissileToTarget()
{
	float length = FVector::Distance(TargetLocation,GetActorLocation());
	FRotator trailRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLocation);
	AActor* trail = GetWorld()->SpawnActor<AActor>(TrailFactory);
	if (trail)
	{
		trail->SetActorLocation(GetActorLocation());
		trail->SetActorRotation(trailRotation);
		trail->SetActorScale3D(FVector(length,1,1));

		FTimerHandle TrailDestroyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TrailDestroyTimerHandle,[this,trail]()
		{
			trail->Destroy();
			this->Destroy();
		},LockOnTime,false);
	}

	FTimerHandle MissileSpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MissileSpawnTimerHandle,[this]()
	{
		FTransform t = FirePos->GetComponentTransform();
		t.SetScale3D(FVector(1.0));
		auto* missile = GetWorld()->SpawnActor<ADiscMissile>(MissileFactory,t);
		if (missile)
		{
			FVector dir = (TargetLocation - GetActorLocation()).GetSafeNormal();
			missile->SetFireDirection(dir);
		}
	},LockOnTime + 0.1,false);
	
}

FVector2D ADisc::GetRandomPointInCircle(float radius, FVector2D centerPoint)
{
	float RandomAngle = FMath::RandRange(0.0f,2.0f*PI);
	float RandomRadius = radius * FMath::Sqrt(FMath::FRand());

	float X = centerPoint.X + RandomRadius * FMath::Cos(RandomAngle);
	float Y = centerPoint.Y + RandomRadius * FMath::Sin(RandomAngle);

	return FVector2D(X,Y);
}

FVector ADisc::GetRandomPointInCircleXY(float radius, FVector centerPoint)
{
	FVector2D random2D = GetRandomPointInCircle(radius,FVector2D(centerPoint.X,centerPoint.Y));
	return FVector(random2D.X,random2D.Y,centerPoint.Z);
}