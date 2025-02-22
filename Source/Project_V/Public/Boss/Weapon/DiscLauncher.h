// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Weapon/BossWeapon.h"
#include "DiscLauncher.generated.h"

UCLASS()
class PROJECT_V_API ADiscLauncher : public ABossWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADiscLauncher();

	UFUNCTION()
	void CreateDisc(FTransform transfrom);
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitComponents();
	UFUNCTION()
	void OnDiscLauncherOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TSubclassOf<class ADisc> DiscFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LeftorRight;
	
};
