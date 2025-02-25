// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/PlayerCameraMode.h"
#include "PlayerBaseComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnEventChangedCameraMode, EPlayerCameraMode);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBaseComponent();

protected:
	FOnEventChangedCameraMode onEventCameraModeChanged;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	class APlayCharacter* me;

	UPROPERTY()
	class UPlayerAnimInstance* anim;
	
public:
	template<typename UserClass>
	void AddBaseEventHandler(UserClass* obj, void (UserClass::* func)(EPlayerCameraMode mode))
	{
		if (obj && func)
		{
			onEventCameraModeChanged.BindUObject(obj, func);
		}
	}
	
	virtual void SetupInputBinding(class UEnhancedInputComponent* input) {}

	virtual void OnChangedCameraMode(EPlayerCameraMode mode) {}
};
