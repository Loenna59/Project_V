// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

UCLASS()
class PROJECT_V_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* arrowSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class APlayerProjectile> projectileFactory;
	
	UPROPERTY()
	TWeakObjectPtr<class APlayerProjectile> projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USkeletalMesh* skeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> animInstance;

	UAnimInstance* GetAnimInstance();

	virtual void SpawnArrowInBow();

	virtual void SpawnArrow(USceneComponent* parent, FName socketName = NAME_None);

	virtual void PlaceArrowOnBow();

	virtual void PlaceOrSpawnArrow();

	virtual bool Fire(FVector direction, float alpha);
	
	virtual void AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow);
};
