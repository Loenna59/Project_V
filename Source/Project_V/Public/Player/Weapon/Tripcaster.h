// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerRangedWeapon.h"
#include "Tripcaster.generated.h"

UCLASS()
class PROJECT_V_API ATripcaster : public APlayerRangedWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATripcaster();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxLength = 1600;

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

	virtual FName GetSlotSocket() const override
	{
		return TEXT("CasterSocket");
	}

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
