// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UWeaponAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsAnchored;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTransform bone;
};
