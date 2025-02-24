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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AWire> wireFactory;
	
	UPROPERTY()
	TWeakObjectPtr<class AWire> wire;

	virtual void SpawnArrowInBow() override;

	virtual void SpawnArrow(USceneComponent* parent, FName socketName = NAME_None) override;

	virtual void AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow) override;

	virtual void PlaceArrowOnBow() override;

	virtual bool Fire(FVector direction, float alpha) override;

	virtual void SetVisibility(bool visible) override;

	virtual void RevertProjectile() override;

	virtual FName GetGripSocket() const override
	{
		return TEXT("CasterGripSocket");
	}

	virtual FName GetPickProjectileSocket() const override
	{
		return TEXT("PickWireSocket");
	}

	virtual EWeaponType GetWeaponType() const override
	{
		return EWeaponType::Caster;
	}
};
