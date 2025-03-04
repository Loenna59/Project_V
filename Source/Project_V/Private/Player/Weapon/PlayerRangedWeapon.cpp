// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/PlayerRangedWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Weapon/PlayerProjectile.h"

UAnimInstance* APlayerRangedWeapon::GetAnimInstance()
{
	if (mesh)
	{
		UAnimInstance* anim = mesh->GetAnimInstance();
		return anim;

	}

	return nullptr;
}

void APlayerRangedWeapon::SpawnArrowInBow()
{
	if (projectile.IsValid())
	{
		return;
	}
	
	APlayerProjectile* spawned_proj = GetWorld()->SpawnActor<APlayerProjectile>(projectileFactory);
	projectile = spawned_proj;
	PlaceArrowOnBow();
}

void APlayerRangedWeapon::SpawnArrow(USceneComponent* parent, FName socketName)
{
	if (projectile.IsValid())
	{
		return;
	}

	APlayerProjectile* spawned_proj = GetWorld()->SpawnActor<APlayerProjectile>(projectileFactory);
	spawned_proj->AttachToComponent(parent, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
	projectile = spawned_proj;
}

void APlayerRangedWeapon::PlaceArrowOnBow()
{
	if (projectile.IsValid())
	{
		projectile->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		projectile->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		projectile->AttachToComponent(arrowSlot, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void APlayerRangedWeapon::PlaceOrSpawnArrow()
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

bool APlayerRangedWeapon::Fire(FVector direction, float alpha)
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

void APlayerRangedWeapon::AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow)
{
	Super::AttachSocket(comp, socketName, visibleArrow);

	if (projectile.IsValid())
	{
		if (projectile->mesh)
		{
			projectile->mesh->SetVisibility(visibleArrow);
		}
	}
}

void APlayerRangedWeapon::SetVisibility(bool visible)
{
	Super::SetVisibility(visible);

	if (projectile.IsValid())
	{
		projectile->mesh->SetVisibility(visible);
	}
}

