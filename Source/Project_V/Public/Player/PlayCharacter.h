// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float walkSpeed = 600;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float sprintSpeed = 1200;

	UFUNCTION()
	void Move(const FInputActionValue& actionValue);

	UFUNCTION()
	void Rotate(const FInputActionValue& actionValue);

	UFUNCTION()
	void ActionJump(const FInputActionValue& actionValue);

	UFUNCTION()
	void Sprint(const FInputActionValue& actionValue);

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

	FVector direction;
};
