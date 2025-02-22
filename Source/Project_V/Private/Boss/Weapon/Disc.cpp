// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/Disc.h"

#include "Project_V.h"
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
	PerposeLocation = GetActorLocation() + (GetActorForwardVector() * 3000.0f);
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
		Mesh->SetRelativeScale3D(FVector(0.4));
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
		LockOnTrailFactory = tempTrail.Class;
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
		LockOnTarget();
		return;
	}
	
	FVector p0 = GetActorLocation();
	FVector direction = (PerposeLocation - GetActorLocation()).GetSafeNormal();
	FVector vt = direction * Speed * GetWorld()->GetDeltaSeconds();
	SetActorLocation(p0 + vt);
}

void ADisc::LockOnTarget()
{
	PRINTLOG(TEXT("LockOnTarget"));
	float length = FVector::Distance(TargetLocation,GetActorLocation());
	FRotator trailRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLocation);
	AActor* trail = GetWorld()->SpawnActor<AActor>(LockOnTrailFactory);
	if (trail)
	{
		trail->SetActorLocation(GetActorLocation());
		trail->SetActorRotation(trailRotation);
		trail->SetActorScale3D(FVector(length,1,1));
	}
	
}
