// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/PlayerWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Weapon/PlayerProjectile.h"

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
	
	APlayerProjectile* spawned_proj = GetWorld()->SpawnActor<APlayerProjectile>(projectileFactory);
	projectile = spawned_proj;
	PlaceArrowOnBow();
}

void APlayerWeapon::SpawnArrow(USceneComponent* parent, FName socketName)
{
	if (projectile.IsValid())
	{
		return;
	}

	APlayerProjectile* spawned_proj = GetWorld()->SpawnActor<APlayerProjectile>(projectileFactory);
	spawned_proj->AttachToComponent(parent, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
	projectile = spawned_proj;
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
		if (fireSfx)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), fireSfx, GetActorLocation());
		}
		projectile->Fire(direction, alpha, false);
		projectile = nullptr;
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

void APlayerWeapon::SetVisibility(bool visible)
{
	bool hidden = !visible;
	SetActorHiddenInGame(hidden);

	if (projectile.IsValid())
	{
		projectile->mesh->SetVisibility(visible);
	}
}

