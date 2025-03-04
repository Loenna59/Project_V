// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/PlayerAnimInstance.h"
#include "PlayerWeapon.generated.h"

UCLASS()
class PROJECT_V_API APlayerWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerWeapon();
	
	virtual void AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow);

	virtual void SetVisibility(bool visible);

	virtual bool IsBase() const
	{
		return true;
	}

	virtual FName GetSlotSocket() const
	{
		return TEXT("SheathSocket");
	}

	virtual FName GetGripSocket() const
	{
		return TEXT("WeaponGripSocket");
	}

	virtual EWeaponType GetWeaponType() const
	{
		return EWeaponType::Melee;
	}
};
