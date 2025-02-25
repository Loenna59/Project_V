// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayCharacter.h"
#include "Player/Component/PlayerBaseComponent.h"
#include "PlayerMovement.generated.h"

DECLARE_DELEGATE_OneParam(FOnEventCameraSlowMode, bool);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UPlayerMovement : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
	FOnEventCameraSlowMode onEventCameraSlowMode;

public:
	// Sets default values for this component's properties
	UPlayerMovement();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float walkSpeed = 600;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float sprintSpeed = 1200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float dodgeSpeed = 800;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float strollSpeed = 400;

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

	virtual void SetupInputBinding(class UEnhancedInputComponent* input) override;

	virtual void OnChangedCameraMode(EPlayerCameraMode mode) override;

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
	void StartDodge();

	UFUNCTION()
	void PlayingDodge();

	UFUNCTION()
	void EndDodge();

	bool bIsPlayingDodge = false;

	template<typename UserClass>
	void AddHandler(UserClass* obj, void (UserClass::* func)(bool value))
	{
		if (obj && func)
		{
			onEventCameraSlowMode.BindUObject(obj, func);
		}
	}
	
protected:
	FVector2D dodgeValue;
	
	UPROPERTY()
	class UCharacterMovementComponent* movementComp;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
