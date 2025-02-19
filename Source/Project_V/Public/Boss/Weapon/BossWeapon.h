// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossWeapon.generated.h"

UCLASS()
class PROJECT_V_API ABossWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABossWeapon();

	UFUNCTION()
	void DamageWeaponHP(float Damage);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxHP;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CurrentHP;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsBroken{false};
};
