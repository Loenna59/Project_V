// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float walkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool shouldMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isFalling;
};
