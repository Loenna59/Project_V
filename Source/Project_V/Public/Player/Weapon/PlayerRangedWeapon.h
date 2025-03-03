// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/PlayerAnimInstance.h"
#include "PlayerRangedWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteFire, bool, complete);

UCLASS()
class PROJECT_V_API APlayerRangedWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	FOnCompleteFire onCompleteFire;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* arrowSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class APlayerProjectile> projectileFactory;
	
	UPROPERTY()
	TWeakObjectPtr<class APlayerProjectile> projectile;

	UPROPERTY(EditDefaultsOnly, Category=SoundEffect)
	class USoundBase* fireSfx;

	UAnimInstance* GetAnimInstance();

	virtual void SpawnArrowInBow();

	virtual void SpawnArrow(USceneComponent* parent, FName socketName = NAME_None);

	virtual void PlaceArrowOnBow();

	virtual void PlaceOrSpawnArrow();

	virtual bool Fire(FVector direction, float alpha);
	
	virtual void AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow);

	virtual void SetVisibility(bool visible);

	virtual bool IsBase() const
	{
		return false;
	}

	virtual FName GetSlotSocket() const
	{
		return TEXT("BowSocket");
	}

	virtual FName GetGripSocket() const
	{
		return TEXT("BowGripSocket");
	}

	virtual FName GetPickProjectileSocket() const
	{
		return TEXT("PickArrowSocket");
	}

	virtual EWeaponType GetWeaponType() const
	{
		return EWeaponType::Base;
	}

	virtual void RevertProjectile() {}
};
