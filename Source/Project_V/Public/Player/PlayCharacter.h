// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraMode.h"
#include "GameFramework/Character.h"
#include "PlayCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventCameraModeChanged, EPlayerCameraMode)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowGameStateUI, bool, gameClear);
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

	FOnEventCameraModeChanged onEventCameraModeChanged;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// TODO
	// 장진혁이 함 나중에 고쳐주세용
	UPROPERTY(BlueprintAssignable)
	FShowGameStateUI ShowGameStateUI;

	UPROPERTY()
	class UPlayerAnimInstance* anim;

	UPROPERTY(VisibleAnywhere)
	class UPlayerMovement* movementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerCombat* combatComp;

	UPROPERTY(VisibleAnywhere)
	class UPlayerCameraSwitcher* cameraSwitcher;

	UPROPERTY(VisibleAnywhere)
	class UPlayerFocusMode* focusMode;

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
	class UAudioComponent* voiceComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* sheath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPlayerUI* ui;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Settings)
	double minPinchDegrees = 50;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Status)
	float maxHealth = 100;

	EPlayerCameraMode prevCameraMode = EPlayerCameraMode::Default;
	
	EPlayerCameraMode currentCameraMode = EPlayerCameraMode::Default;

	bool bIsBlockControl = false;
	
private:
	UPROPERTY()
	class UInputMappingContext* imc;

	float currentHealth;

	FTimerHandle voicePlayTimerHandle;

	UPROPERTY()
	class USoundBase* damagedVoice;
	
	float audioPlayTime = 1.f;

	void TakeDamageInternal(float damage, FVector forward, float& degrees);

public:
	template<typename UserClass>
	void AddEventHandler(UserClass* obj, void (UserClass::* func)(EPlayerCameraMode mode))
	{
		if (obj && func)
		{
			onEventCameraModeChanged.AddUObject(obj, func);
		}
	}

	EPlayerCameraMode GetPlayerCameraMode() const
	{
		return currentCameraMode;
	}

	void SetPlayingDodge(bool isPlaying);

	void SetCurrentHealth(float health);

	void SetPlayerCameraMode(EPlayerCameraMode mode);

	bool IsNotAnchoredMode();

	void HitDamage(float damage, FVector forward);

	void HitLargeDamage(float damage, FVector forward);

	UFUNCTION(BlueprintCallable)
	void GameOver();

	UFUNCTION(BlueprintCallable)
	void SetVisible(bool visible);
};
