// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Component/PlayerBaseComponent.h"
#include "PlayerFocusMode.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UPlayerFocusMode : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerFocusMode();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	class UInputAction* ia_focus;

	UPROPERTY()
	class UInputAction* ia_toggle;

	float focusPressingTime;

	bool bIsTracking;

public:
	virtual void SetupInputBinding(class UEnhancedInputComponent* input) override;

	virtual void OnChangedCameraMode(EPlayerCameraMode mode) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AFocusDome> domeFactory;

	UPROPERTY(VisibleAnywhere)
	class AFocusDome* focusDome;

	UPROPERTY(VisibleAnywhere)
	class AThunderJaw* Boss;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float focusModeThreshold = 0.5f;
	
	UFUNCTION()
	void OnFocusOrScan(const FInputActionValue& actionValue);

	UFUNCTION()
	void EndFocusOrScan();

	UFUNCTION()
	void OnToggleTracking();

	void SetVisibleFocusMode(bool visible);
};
