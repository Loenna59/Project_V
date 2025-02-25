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
	void CreateBullet(FTransform transform, FVector Target);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitComponents();
	UFUNCTION(BlueprintCallable)
	void OnMachineGunOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AMachineGunBullet> BulletFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomSprayRadius{300.0f};

	UPROPERTY()
	float LeftorRight;
};
