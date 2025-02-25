// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerCameraSwitcher.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UPlayerCameraSwitcher : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerCameraSwitcher();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	const float maxFOV = 90;
	const float minFOV = 40;

	float currentBlendCameraAlpha;
	
	float targetBlendCameraAlpha;

	float targetFOV;

	float targetMultiplier;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnChangedCameraMode(EPlayerCameraMode mode) override;
	
	UPROPERTY()
	class USpringArmComponent* springArmComp;

	UPROPERTY()
	class UCameraComponent* cameraComp;

	UPROPERTY()
	class USpringArmComponent* anchoredSpringArmComp;

	UPROPERTY()
	class UCameraComponent* anchoredCameraComp;

	UPROPERTY()
	class USpringArmComponent* transitionSpringArmComp;

	UPROPERTY()
	class UCameraComponent* transitionCameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float cameraTransitionSpeedMultiplier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float slowDilation = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float slowMotionMultiplier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float releaseMotionMultiplier = 10.f;

	void SetCameraSlowMode(bool bActive);
};
