// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "Player/PlayerWeapon.h"
#include "PlayerCombat.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnEventReleased);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UPlayerCombat : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerCombat();

protected:
	FOnEventReleased onEventReleased;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector CalculateAnimToVector();

	float drawStrength = 0;
	
	float elapsedDrawingTime; // 활 시위 경과 시간

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float targetDrawStrength = 100.0f; // 활시위 최대값

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float drawDuration = 1.f; // 활시위가 최대로 당길 때 까지 걸리는 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float drawingThreshold = 1;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* input) override;
	
	template<typename UserClass>
	void AddHandler(UserClass* obj, void (UserClass::* func)())
	{
		if (obj && func)
		{
			onEventReleased.AddUObject(obj, func);
		}
	}

	TWeakObjectPtr<APlayerWeapon> holdingWeapon;
	
	UPROPERTY()
	class UInputAction* ia_anchored;

	UPROPERTY()
	class UInputAction* ia_fire;

	UPROPERTY()
	class UCameraComponent* cameraComp;

	UFUNCTION()
	void OnAnchor();

	UFUNCTION()
	void OnAnchorRelease();

	UFUNCTION()
	void OnPressedFire(const FInputActionValue& actionValue);

	UFUNCTION()
	void OnReleasedFire(const FInputActionValue& actionValue);

	bool bIsCompleteReload = false;

	float GetDrawStrength() const
	{
		return drawStrength;
	}

	void SetDrawStrength(float strength);
};
