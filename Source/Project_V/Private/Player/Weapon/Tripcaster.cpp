// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Tripcaster.h"
#include "Player/Weapon/PlayerProjectile.h"

// Sets default values
ATripcaster::ATripcaster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(mesh);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Ranged/SKM_Crossbow.SKM_Crossbow'"));

	if (skeletalMesh.Succeeded())
	{
		mesh->SetSkeletalMesh(skeletalMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> temp_anim (TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Tripcaster.ABP_Tripcaster_C'"));

	if (temp_anim.Succeeded())
	{
		mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		mesh->SetAnimInstanceClass(temp_anim.Class);
	}

	ConstructorHelpers::FClassFinder<APlayerProjectile> temp_proj (TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_Wire.BP_Wire_C'"));

	if (temp_proj.Succeeded())
	{
		projectileFactory = temp_proj.Class;
	}

	arrowSlot = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSlotComp"));
	arrowSlot->SetupAttachment(mesh, TEXT("bowstring"));
	arrowSlot->SetRelativeLocation(FVector(25, 0, 0));
	arrowSlot->SetRelativeScale3D(FVector(1.1f));
}
