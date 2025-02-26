// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiscMissile.generated.h"

UCLASS()
class PROJECT_V_API ADiscMissile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADiscMissile();

	UFUNCTION()
	void SetFireDirection(const FVector& Direction);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitComponents();
	
	UFUNCTION()
	void OnDiscMissileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UBoxComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* PMC;

	UPROPERTY()
	FVector PerposeDirection;
};
