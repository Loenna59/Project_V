// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* weaponComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* arrowSlotComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AArrow> arrowFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float walkSpeed = 600;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float sprintSpeed = 1200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float dodgeSpeed = 800;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	double minPinchDegrees = 50;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float cameraTransitionSpeedMultiplier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float targetDrawStrength = 100.0f; // 활시위 최대값

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float drawDuration = 1.f; // 활시위가 최대로 당길 때 까지 걸리는 시간

	FVector2D dodgeAxis;

	bool bIsDodge = false;
	
	bool bIsAnchored = false;

	FVector direction;
	
	float drawStrength = 0;

	bool bIsShot = false;

	UFUNCTION()
	void Move(const FInputActionValue& actionValue);

	UFUNCTION()
	void Rotate(const FInputActionValue& actionValue);

	UFUNCTION()
	void ActionJump(const FInputActionValue& actionValue);

	UFUNCTION()
	void Sprint(const FInputActionValue& actionValue);

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

	void SpawnArrow();
	
private:
	UPROPERTY()
	class UInputMappingContext* imc;

	UPROPERTY()
	class UInputAction* ia_move;

	UPROPERTY()
	class UInputAction* ia_rotate;

	UPROPERTY()
	class UInputAction* ia_jump;

	UPROPERTY()
	class UInputAction* ia_sprint;

	UPROPERTY()
	class UInputAction* ia_movePressed;

	UPROPERTY()
	class UInputAction* ia_doubleTap;

	UPROPERTY()
	class UInputAction* ia_anchored;

	UPROPERTY()
	class UInputAction* ia_fire;

	float currentBlendCameraAlpha;
	
	float targetBlendCameraAlpha;

	float elapsedDrawingTime; // 활 시위 경과 시간

	UPROPERTY()
	TWeakObjectPtr<class AArrow> arrow;
};
