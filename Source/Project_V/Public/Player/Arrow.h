// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class PROJECT_V_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	UFUNCTION()
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UProjectileMovementComponent* projectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* tailVFX;

	UPROPERTY(EditAnywhere, Category=Settings)
	float initialSpeed = 5000;

	UPROPERTY(EditAnywhere, Category=Settings)
	float maxSpeed = 5000;
	
	UFUNCTION()
	void Fire(FVector to, float alpha);

	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
