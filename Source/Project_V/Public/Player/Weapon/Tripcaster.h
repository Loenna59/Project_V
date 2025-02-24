// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapon.h"
#include "Tripcaster.generated.h"

UCLASS()
class PROJECT_V_API ATripcaster : public APlayerWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATripcaster();

	virtual FName GetGripSocket() const override
	{
		return TEXT("CasterGripSocket");
	}

	virtual EWeaponType GetWeaponType() const override
	{
		return EWeaponType::Caster;
	}
};
