// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FocusDome.generated.h"

UCLASS()
class PROJECT_V_API AFocusDome : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFocusDome();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float expandScale = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float timeMultiplier = 5;

	UFUNCTION()
	void Activate();

	UFUNCTION()
	void Deactivate();

private:
	const float minimalizeScale = 0.1f;
	
	float destScale;
	
	float currentScale;
};
