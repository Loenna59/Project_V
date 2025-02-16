// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MachineGunBullet.generated.h"

UCLASS()
class PROJECT_V_API AMachineGunBullet : public AActor
{
	GENERATED_BODY()

public:
	AMachineGunBullet();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitComponents();
	
	UFUNCTION()
	void OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION()
	void FireInDirection(const FVector& Direction);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UBoxComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float LifeTime{10.0f};
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* PMC;
};
