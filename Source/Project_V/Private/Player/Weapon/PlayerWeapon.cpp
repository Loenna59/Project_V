// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/PlayerWeapon.h"

#include "Player/Weapon/Arrow.h"

UAnimInstance* APlayerWeapon::GetAnimInstance()
{
	if (mesh)
	{
		UAnimInstance* anim = mesh->GetAnimInstance();
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

