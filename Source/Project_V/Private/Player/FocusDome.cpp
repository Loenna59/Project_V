// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FocusDome.h"

// Sets default values
AFocusDome::AFocusDome()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SetRootComponent(sphere);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tmp_mesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	if (tmp_mesh.Succeeded())
	{
		sphere->SetStaticMesh(tmp_mesh.Object);
	}
	
	ConstructorHelpers::FObjectFinder<UMaterialInstance> tmp_material(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_FocusMode_Inst.M_FocusMode_Inst'"));

	if (tmp_material.Succeeded())
	{
		sphere->SetMaterial(0, tmp_material.Object);
	}

	sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	destScale = minimalizeScale;
}

// Called when the game starts or when spawned
void AFocusDome::BeginPlay()
{
	Super::BeginPlay();

	currentScale = destScale;
	
	sphere->SetRelativeScale3D(FVector(destScale));
	sphere->SetVisibility(false);
}

// Called every frame
void AFocusDome::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentScale != destScale)
	{
		currentScale = FMath::Lerp(currentScale, destScale, 0.1f * timeMultiplier);

		currentScale = FMath::Clamp(currentScale, minimalizeScale, expandScale);

		sphere->SetRelativeScale3D(FVector(currentScale));
		sphere->SetVisibility(currentScale > minimalizeScale);
	}

	// APawn* player = GetWorld()->GetFirstPlayerController()->GetPawn();
	//
	// sphere->SetVectorParameterValueOnMaterials(TEXT("Player Position"), player->GetActorLocation());
	// sphere->SetVectorParameterValueOnMaterials(TEXT("Player Forward Vector"), player->GetActorForwardVector());
}

void AFocusDome::Activate()
{
	destScale = expandScale;
}

void AFocusDome::Deactivate()
{
	destScale = minimalizeScale;
}

