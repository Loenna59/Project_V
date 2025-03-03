// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Weapon/Katana.h"

// Sets default values
AKatana::AKatana()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

