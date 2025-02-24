// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerProjectile.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class PROJECT_V_API AArrow : public APlayerProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	virtual void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
