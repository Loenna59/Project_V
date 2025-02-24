// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerWeapon.h"

#include "Player/Arrow.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(mesh);

	if (skeletalMesh)
	{
		mesh->SetSkeletalMesh(skeletalMesh);
	}

	if (animInstance)
	{
		mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		mesh->SetAnimInstanceClass(animInstance);
	}

	arrowSlot = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSlotComp"));
	arrowSlot->SetupAttachment(mesh, TEXT("bowstring"));
	arrowSlot->SetRelativeLocation(FVector(25, 0, 0));
	arrowSlot->SetRelativeScale3D(FVector(1.1f));

}

UAnimInstance* APlayerWeapon::GetAnimInstance()
{
	if (mesh)
	{
		UAnimInstance* anim = mesh->GetAnimInstance();
		if (!anim)
		{
			if (animInstance)
			{
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				mesh->SetAnimInstanceClass(animInstance);
			}
			anim = mesh->GetAnimInstance();
		}
		
		return anim;

	}

	return nullptr;
}

void APlayerWeapon::SpawnArrowInBow()
{
	if (projectile.IsValid())
	{
		return;
	}
	
	AArrow* spawned_arrow = GetWorld()->SpawnActor<AArrow>(projectileFactory);
	projectile = spawned_arrow;
	PlaceArrowOnBow();
}

void APlayerWeapon::SpawnArrow(USceneComponent* parent, FName socketName)
{
	if (projectile.IsValid())
	{
		return;
	}

	AArrow* spawned_arrow = GetWorld()->SpawnActor<AArrow>(projectileFactory);
	spawned_arrow->AttachToComponent(parent, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
	projectile = spawned_arrow;
}

void APlayerWeapon::PlaceArrowOnBow()
{
	if (projectile.IsValid())
	{
		projectile->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		projectile->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		projectile->AttachToComponent(arrowSlot, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void APlayerWeapon::PlaceOrSpawnArrow()
{
	if (projectile.IsValid())
	{
		PlaceArrowOnBow();
	}
	else
	{
		SpawnArrowInBow();
	}
}

bool APlayerWeapon::Fire(FVector direction, float alpha)
{
	if (projectile.IsValid())
	{
		projectile->Fire(direction, alpha);
		projectile = nullptr;

		return false;
	}

	return true;
}

void APlayerWeapon::AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow)
{
	AttachToComponent(comp, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);

	if (projectile.IsValid())
	{
		if (projectile->mesh)
		{
			projectile->mesh->SetVisibility(visibleArrow);
		}
	}
}

