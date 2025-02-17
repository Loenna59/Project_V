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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* arrowSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AArrow> arrowFactory;
	
	UPROPERTY()
	TWeakObjectPtr<class AArrow> arrow;

	UAnimInstance* GetAnimInstance();

	void SpawnArrowInBow();

	void SpawnArrow(USceneComponent* parent, FName socketName = NAME_None);

	void PlaceArrowOnBow();

	void PlaceOrSpawnArrow();

	bool Fire(FVector direction, float alpha);
};
