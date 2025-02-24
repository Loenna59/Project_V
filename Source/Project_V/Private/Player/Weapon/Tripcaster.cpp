// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Tripcaster.h"

#include "Project_V.h"
#include "Player/Weapon/PlayerProjectile.h"
#include "Player/Weapon/Wire.h"

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

	ConstructorHelpers::FClassFinder<AWire> temp_proj (TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_Wire.BP_Wire_C'"));

	if (temp_proj.Succeeded())
	{
		wireFactory = temp_proj.Class;
	}

	ConstructorHelpers::FClassFinder<APlayerProjectile> temp_proj2 (TEXT("/Script/Engine.BlueprintGeneratedClass'/Game/Blueprints/Player/BP_Arrow.BP_Arrow_C'"));

	if (temp_proj2.Succeeded())
	{
		projectileFactory = temp_proj2.Class;
	}

	arrowSlot = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSlotComp"));
	arrowSlot->SetupAttachment(mesh, TEXT("bowstring"));
	arrowSlot->SetRelativeLocation(FVector(25, 0, 0));
	arrowSlot->SetRelativeScale3D(FVector(1.1f));
}

void ATripcaster::SpawnArrowInBow()
{
	if (!wire.IsValid())
	{
		AWire* spawned_proj = GetWorld()->SpawnActor<AWire>(wireFactory);
		wire = spawned_proj;
	}
	
	if (!projectile.IsValid())
	{
		APlayerProjectile* spawned_proj2 = GetWorld()->SpawnActor<APlayerProjectile>(projectileFactory);
		projectile = spawned_proj2;
		if (projectile->mesh)
		{
			projectile->mesh->SetVisibility(false);
		}
	}

	PlaceArrowOnBow();
}

void ATripcaster::SpawnArrow(USceneComponent* parent, FName socketName)
{
	if (wire.IsValid() && wire->IsChaining())
	{
		if (projectile.IsValid())
		{
			projectile->AttachToComponent(parent, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
			if (projectile->mesh)
			{
				projectile->mesh->SetVisibility(true);
			}
		}
		return;	
	}
	
	if (!wire.IsValid())
	{
		AWire* spawned_proj2 = GetWorld()->SpawnActor<AWire>(wireFactory);
		spawned_proj2->AttachToComponent(parent, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
		wire = spawned_proj2;
	}
	
	if (!projectile.IsValid())
	{
		APlayerProjectile* spawned_proj = GetWorld()->SpawnActor<APlayerProjectile>(projectileFactory);
		projectile = spawned_proj;
		if (projectile->mesh)
		{
			projectile->mesh->SetVisibility(false);
		}
	}
}

void ATripcaster::AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow)
{
	Super::AttachSocket(comp, socketName, visibleArrow);

	if (wire.IsValid())
	{
		if (wire->mesh)
		{
			wire->mesh->SetVisibility(visibleArrow);
		}
	}
}

void ATripcaster::PlaceArrowOnBow()
{
	if (wire.IsValid())
	{
		if (projectile.IsValid() && wire->IsChaining())
		{
			projectile->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			projectile->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
			projectile->AttachToComponent(arrowSlot, FAttachmentTransformRules::SnapToTargetIncludingScale);

			return;
		}

		wire->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		wire->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		wire->AttachToComponent(arrowSlot, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

bool ATripcaster::Fire(FVector direction, float alpha)
{
	if (wire.IsValid())
	{
		if (projectile.IsValid() && wire->IsChaining())
		{
			projectile->Fire(direction, alpha);
			projectile = nullptr;
			wire = nullptr;

			return true;
		}
		
		wire->Fire(direction, alpha);
		wire->Link(projectile.Get());
	}
	
	return false;
}

void ATripcaster::SetVisibility(bool visible)
{
	Super::SetVisibility(visible);
}

void ATripcaster::RevertProjectile()
{
	if (wire.IsValid())
	{
		wire->Link(nullptr);
		
		wire->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		wire->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		wire->AttachToComponent(arrowSlot, FAttachmentTransformRules::SnapToTargetIncludingScale);

		if (projectile.IsValid())
		{
			if (projectile->mesh)
			{
				projectile->mesh->SetVisibility(false);
			}
		}
	}
}
