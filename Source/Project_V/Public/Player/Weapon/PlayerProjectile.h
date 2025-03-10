// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerProjectile.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEventComponentHit);

UCLASS()
class PROJECT_V_API APlayerProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	bool bOrtho = false;

	class UParticleSystem* hitFx;
	
public:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* moveComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UParticleSystem* tailVFX;

	UPROPERTY(EditAnywhere, Category=Settings)
	float initialSpeed = 5000;

	UPROPERTY(EditAnywhere, Category=Settings)
	float maxSpeed = 5000;

	UFUNCTION()
	virtual void Fire(FVector to, float alpha, bool ortho);

	UFUNCTION()
	virtual void DestroyAfterPlayFX() {}

	FOnEventComponentHit onEventComponentHit;
	
};
