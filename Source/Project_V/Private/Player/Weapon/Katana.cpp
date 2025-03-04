// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Weapon/Katana.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"

// Sets default values
AKatana::AKatana()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tmp_mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/CyberpunkSamurai/Meshes/SM_Katana.SM_Katana'"));

	if (tmp_mesh.Succeeded())
	{
		mesh->SetStaticMesh(tmp_mesh.Object);
	}
}

// Called when the game starts or when spawned
void AKatana::BeginPlay()
{
	Super::BeginPlay();
}

void AKatana::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (isAttacking)
	{
		FCollisionQueryParams params;

		params.AddIgnoredActor(this);
		
		FHitResult hitResult;
		bool hit = GetWorld()->SweepSingleByChannel(
			hitResult,
			GetActorLocation(),
			GetActorLocation() + FVector(0, 0, 100),
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeBox(FVector(10.f)),
			params
		);

		if (hit)
		{
			AActor* hitActor = hitResult.GetActor();
			if (AThunderJaw* boss = Cast<AThunderJaw>(hitActor))
			{
				boss->BossTakeDamage(1);
			}
		}

		// DrawDebugBox(GetWorld(), GetActorLocation() + FVector(0, 0, 50), FVector(10.f), FColor::Yellow, true, 5);
	}
}

void AKatana::StartTraceChannel()
{
	isAttacking = true;
}

void AKatana::EndTraceChannel()
{
	isAttacking = false;
}

