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

	UFUNCTION()
	void SetBulletActive(bool value) const;
	UFUNCTION()
	void FireToTarget(const FVector& Target, float Radius);
	UFUNCTION()
	void BackToMagazine();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayGunSound();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitComponents();
	
	UFUNCTION()
	void OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	FVector2D GetRandomPointInCircle(float radius, FVector2D centerPoint);

	UFUNCTION()
	FVector GetRandomPointInCircleXY(float radius, FVector centerPoint);

public:
	UPROPERTY(EditAnywhere)
	class AMachineGun* MachineGun;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UBoxComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float LifeTime{3.0f};
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* PMC;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class USoundWave* GunSound;
};
