// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerCombat.generated.h"

UENUM(BlueprintType)
enum class KatanaPlayState : uint8
{
	Unequipped,
	Holding,
	Acting
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventReleased, bool);
DECLARE_DELEGATE_RetVal(bool, FOnEventCheckCameraMode)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UPlayerCombat : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerCombat();

	KatanaPlayState katanaPlayState = KatanaPlayState::Unequipped;

protected:
	FOnEventReleased onEventReleased;
	
	FOnEventCheckCameraMode onEventCheckCameraMode;
	
	FTimerHandle timerHandle;

	float drawStrength = 0;
	
	float elapsedDrawingTime;

	bool bIsCompleteReload = false; // 활 시위 경과 시간

	FVector CalculateAnimToVector();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(VisibleAnywhere)
	class AKatana* katana;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayerRangedWeapon* bow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayerRangedWeapon* tripcaster;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class APlayerRangedWeapon> bowFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AKatana> katanaFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class APlayerRangedWeapon> tripcasterFactory;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float targetDrawStrength = 100.0f; // 활시위 최대값

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float drawDuration = 1.f; // 활시위가 최대로 당길 때 까지 걸리는 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float drawingThreshold = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	float idleTimerDuration = 10;

	UPROPERTY()
	class UInputAction* ia_anchored;

	UPROPERTY()
	class UInputAction* ia_fire;

	UPROPERTY()
	class UInputAction* ia_quick;

	UPROPERTY()
	class UInputAction* ia_wheel;

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

	UFUNCTION()
	void OnSwapWeapon(const FInputActionValue& actionValue);

	UFUNCTION()
	void OnWheelWeapon(const FInputActionValue& actionValue);

	UFUNCTION()
	void OnMeleeAttack();

	UFUNCTION()
	void CheckPutWeaponTimer(bool bComplete);

	UFUNCTION(BlueprintCallable)
	void ChangeWeapon(class APlayerRangedWeapon* weapon);
	
	TWeakObjectPtr<class APlayerWeapon> holdingWeapon;
	
	TWeakObjectPtr<class APlayerRangedWeapon> currentRangedWeapon;

	void SpawnArrow();

	void PlaceArrowOnBow();

	void PickWeapon();

	void ClearPutWeaponTimer();

	void StartTimerPutWeapon();

	void Fire(FVector velocity, float alpha);

	void SetVisibleEquippedWeapon(bool visible);

	void OnStartTraceKatanaChannel();
	
	void OnEndTraceKatanaChannel();

	void SetDrawStrength(float strength);
	
	float GetDrawStrength() const
	{
		return drawStrength;
	}
	
	virtual void SetupInputBinding(class UEnhancedInputComponent* input) override;

	virtual void OnChangedCameraMode(EPlayerCameraMode mode) override;
	
	template<typename UserClass>
	void AddHandler(UserClass* obj, void (UserClass::* func)(bool active))
	{
		if (obj && func)
		{
			onEventReleased.AddUObject(obj, func);
		}
	}

	template<typename UserClass>
	void AddHandler(UserClass* obj, bool (UserClass::* func)())
	{
		if (obj && func)
		{
			onEventCheckCameraMode.BindUObject(obj, func);
		}
	}
};
