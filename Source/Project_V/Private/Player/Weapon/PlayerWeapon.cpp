// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Weapon/PlayerWeapon.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerWeapon::AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow)
{
	AttachToComponent(comp, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
}

void APlayerWeapon::SetVisibility(bool visible)
{
	bool hidden = !visible;
	SetActorHiddenInGame(hidden);
}

