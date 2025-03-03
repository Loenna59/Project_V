// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerRangedWeapon.h"
#include "Bow.generated.h"

UCLASS()
class PROJECT_V_API ABow : public APlayerRangedWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABow();

	virtual void SetVisibility(bool visible) override;

	virtual bool IsBase() const override
	{
		return true;
	}

	virtual FName GetSlotSocket() const override
	{
		return TEXT("BowSocket");
	}

	virtual FName GetGripSocket() const override
	{
		return TEXT("BowGripSocket");
	}

	virtual EWeaponType GetWeaponType() const override
	{
		return EWeaponType::Base;
	}
};
