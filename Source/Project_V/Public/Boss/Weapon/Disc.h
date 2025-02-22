// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Disc.generated.h"

UCLASS()
class PROJECT_V_API ADisc : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADisc();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitComponents();
	UFUNCTION()
	void MoveToPerposeLocation();
	UFUNCTION()
	void LaunchMissileToTarget();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UBoxComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UArrowComponent* FirePos;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	TSubclassOf<AActor> TrailFactory;	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	TSubclassOf<AActor> MissileFactory;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Settings")
	FVector PerposeLocation;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Settings")
	FVector TargetLocation;	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Settings")
	float AcceptanceRadius{50.0f};
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsArrive{false};
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float Speed{3000.0f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float LockOnTime{2.0};
};
