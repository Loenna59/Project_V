// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapon.h"
#include "Bow.generated.h"

UCLASS()
class PROJECT_V_API ABow : public APlayerWeapon
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
	
};
