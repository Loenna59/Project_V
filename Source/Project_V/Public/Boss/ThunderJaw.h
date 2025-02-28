// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "ThunderJaw.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFloatingDamage,float,damage);

class APlayCharacter;
class UThunderJawFSM;
class AThunderJawAIController;

UCLASS()
class PROJECT_V_API AThunderJaw : public ACharacter
{
	GENERATED_BODY()
public:
	AThunderJaw();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void InitConstruct();
	UFUNCTION()
	void InitBeginPlay();
	UFUNCTION()
	void OnBossBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
#pragma region GetFunc
	UFUNCTION(BlueprintCallable)
	UThunderJawFSM* GetFSMComponent();
	UFUNCTION(BlueprintCallable)
	AThunderJawAIController* GetBossAIController();
	UFUNCTION(BlueprintCallable)
	class UThunderJawAnimInstance* GetBossAnimInstance();
	UFUNCTION(BlueprintCallable)
	class UMaterialInstanceDynamic* GetEyeMatInst();
	UFUNCTION(BlueprintCallable)
	APlayCharacter* GetAloy();
	UFUNCTION(BlueprintCallable)
	class AMachineGun* GetLMachineGun();
	UFUNCTION(BlueprintCallable)
	class AMachineGun* GetRMachineGun();
	UFUNCTION(BlueprintCallable)
	class ADiscLauncher* GetLDiscLauncher();
	UFUNCTION(BlueprintCallable)
	class ADiscLauncher* GetRDiscLauncher();
	UFUNCTION(BlueprintCallable)
	class UWidgetComponent* GetWidgetComponent();
	UFUNCTION(BlueprintCallable)
	class USplineComponent* GetSplineComponent();	
#pragma endregion

#pragma region PartDestructFunc
	UFUNCTION()
	void MachineGunBroken(float LeftOrRight);
	UFUNCTION()
	void DiscLauncherBroken(float LeftOrRight);
#pragma endregion

#pragma region ETC
	UFUNCTION()
	void ChangeEyeColor(FLinearColor Color, float EmissivePower);
	UFUNCTION()
	void RotateToTarget(FVector TargetLoc, float InterpSpeed);
	UFUNCTION()
	static void DrawDebugCircle(const UWorld* World, const FVector& Center, const float Radius);
	UFUNCTION()
	void SpawnDamageUI(const float Damage);
	UFUNCTION(BlueprintCallable)
	void SetVisibilityBoss();
	UFUNCTION(BlueprintCallable)
	void ChangeToFocusModeMat(const bool bFocusMode);
	UFUNCTION(BlueprintCallable)
	void BossTakeDamage(const int Damage);
	UFUNCTION(BlueprintCallable)
	void GameClear();
#pragma endregion
	
protected:
#pragma region ClassInst
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	UThunderJawFSM* FSM;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UNavigationInvokerComponent* NavInvoker;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	AThunderJawAIController* BossAIController;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UThunderJawAnimInstance* BossAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	APlayCharacter* Aloy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UMaterialInstanceDynamic* EyeMatInst;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UWidgetComponent* WidgetComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USplineComponent* SplineComp;
#pragma endregion 

#pragma region PartsClass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class AMachineGun* LMachineGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class AMachineGun* RMachineGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class ADiscLauncher* LDiscLauncher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class ADiscLauncher* RDiscLauncher;
#pragma endregion

#pragma region DamageUI
	UPROPERTY(BlueprintAssignable)
	FFloatingDamage FloatingDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> FloatingTextFactory;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UDamageUI* DamageUI;
#pragma endregion
	
public:
#pragma region SettingVariance
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float MaxHP{1000.0};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float CurrentHP{CurrentHP = MaxHP};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float BossSpeed{500.0f};
#pragma endregion

#pragma region DistanceVariance
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float MeleeAttackDist{1000.0f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float RangeAttackDist{2500.0f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float PatrolDist{2000.0f};
#pragma endregion 

#pragma region Boolean
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	bool bIsLevelSequenceEnd{false};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	bool bPartBroken{false};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	bool bTrapped{false};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	bool bDie{false};
#pragma endregion

#pragma region Sound
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Sound")
	class UAudioComponent* SoundInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundWave* RageSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundWave* WalkSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundWave* ChargeSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundWave* FallDownSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundWave* DieSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundWave* MachineGunSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	class USoundWave* TailSound;
#pragma endregion 
};
