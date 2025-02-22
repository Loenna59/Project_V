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

	UFUNCTION()
	void FlyToPerposeLocation();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitComponents();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UBoxComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	class UArrowComponent* FirePos;
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float speed{3000.0f};
};
