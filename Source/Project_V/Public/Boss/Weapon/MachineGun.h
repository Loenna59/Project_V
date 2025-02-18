// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Weapon/BossWeapon.h"
#include "GameFramework/Actor.h"
#include "MachineGun.generated.h"

UCLASS()
class PROJECT_V_API AMachineGun : public ABossWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMachineGun();

	UFUNCTION()
	void CreateBullet(FTransform transform, FVector direction);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitComponents();
	UFUNCTION()
	void OnMachineGunOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* FirePos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	TSubclassOf<class AMachineGunBullet> BulletFactory;
};
