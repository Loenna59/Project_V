// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraMode.h"
#include "GameFramework/Character.h"
#include "PlayCharacter.generated.h"

UCLASS()
class PROJECT_V_API APlayCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* cameraComp;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* anchoredSpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* anchoredCameraComp;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* transitionSpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* transitionCameraComp;

	UPROPERTY(VisibleAnywhere)
	class APlayerWeapon* bow;

	UPROPERTY(VisibleAnywhere)
	class AFocusDome* focusDome;
	
	class UPlayerUI* ui;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class APlayerWeapon> bowFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AFocusDome> domeFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float strollSpeed = 400;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float walkSpeed = 600;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float sprintSpeed = 1200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float dodgeSpeed = 800;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	double minPinchDegrees = 50;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float cameraTransitionSpeedMultiplier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float targetDrawStrength = 100.0f; // 활시위 최대값

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float drawDuration = 1.f; // 활시위가 최대로 당길 때 까지 걸리는 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float slowDilation = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float slowMotionMultiplier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float releaseMotionMultiplier = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	class UAnimMontage* equipWeaponMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float focusModeThreshold = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float drawingThreshold = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float idleTimerDuration = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Status)
	float maxHealth = 100;

	FVector2D prevDodgeAxis;
	
	FVector2D dodgeAxis;
	
	// FVector direction;

	bool bIsDodge = false;

	bool bIsCompleteReload = false;

	bool bIsPlayingDodge = false;

	bool bIsShot = false;

	UFUNCTION()
	void Move(const FInputActionValue& actionValue);

	UFUNCTION()
	void Rotate(const FInputActionValue& actionValue);

	UFUNCTION()
	void ActionJump(const FInputActionValue& actionValue);

	UFUNCTION()
	void OnTriggerShift(const FInputActionValue& actionValue);

	UFUNCTION()
	void BeginDodge(const FInputActionValue& actionValue);

	UFUNCTION()
	void Dodge();

	UFUNCTION()
	void OnAnchor();

	UFUNCTION()
	void OnAnchorRelease();

	UFUNCTION()
	void OnPressedFire(const FInputActionValue& actionValue);

	UFUNCTION()
	void OnReleasedFire(const FInputActionValue& actionValue);

	UFUNCTION()
	void OnFocusOrScan(const FInputActionValue& actionValue);

	UFUNCTION()
	void EndFocusOrScan();

	void SpawnArrow();

	void PlaceArrowOnBow();

	void PickWeapon();

	void ChangeToDefaultCamera();
	
	void ChangeToAnchoredCamera();
	
private:
	const float maxFOV = 90;
	const float minFOV = 40; 
	
	UPROPERTY()
	class UInputMappingContext* imc;

	UPROPERTY()
	class UInputAction* ia_move;

	UPROPERTY()
	class UInputAction* ia_rotate;

	UPROPERTY()
	class UInputAction* ia_jump;

	UPROPERTY()
	class UInputAction* ia_shift;

	UPROPERTY()
	class UInputAction* ia_movePressed;

	UPROPERTY()
	class UInputAction* ia_doubleTap;

	UPROPERTY()
	class UInputAction* ia_anchored;

	UPROPERTY()
	class UInputAction* ia_fire;

	UPROPERTY()
	class UInputAction* ia_focus;

	TWeakObjectPtr<APlayerWeapon> holdingWeapon;

	float drawStrength = 0;

	float currentBlendCameraAlpha;
	
	float targetBlendCameraAlpha;

	float elapsedDrawingTime; // 활 시위 경과 시간

	float targetFOV;

	float targetMultiplier;

	float currentHealth;

	float focusPressingTime;

	EPlayerCameraMode prevCameraMode = EPlayerCameraMode::Default;
	
	EPlayerCameraMode currentCameraMode = EPlayerCameraMode::Default;

	FVector CalculateAnimToVector();

	FTimerHandle timerHandle;

public:
	float GetDrawStrength() const
	{
		return drawStrength;
	}

	EPlayerCameraMode GetPlayerCameraMode() const
	{
		return currentCameraMode;
	}

	void SetDrawStrength(float strength);

	void SetPlayingDodge(bool isPlaying);

	void SetCurrentHealth(float health);

	void SetPlayerCameraMode(EPlayerCameraMode mode);
};
