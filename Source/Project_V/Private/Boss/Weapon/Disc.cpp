// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/Disc.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
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
	
}

// Called every frame
void ADisc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlyToPerposeLocation();
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
}

void ADisc::FlyToPerposeLocation()
{
	FVector p0 = GetActorLocation();
	FVector direction = GetActorForwardVector();
	FVector vt = direction * speed * GetWorld()->GetDeltaSeconds();
	SetActorLocation(p0 + vt);
}
