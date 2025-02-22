// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThunderJaw.generated.h"

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

public:
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
	
	UFUNCTION()
	void MachineGunBroken(float LeftorRight);
	UFUNCTION()
	void DiscLauncherBroken(float LeftorRight);
	UFUNCTION()
	void ChangeEyeColor(FLinearColor color, float emissivePower);
	UFUNCTION()
	void RotateToTarget(FVector TargetLoc, float InterpSpeed);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	UThunderJawFSM* FSM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UNavigationInvokerComponent* NavInvoker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	AThunderJawAIController* BossAIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UThunderJawAnimInstance* BossAnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UMaterialInstanceDynamic* EyeMatInst;
	UPROPERTY()
	APlayCharacter* Aloy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class AMachineGun* LMachineGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class AMachineGun* RMachineGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class ADiscLauncher* LDiscLauncher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class ADiscLauncher* RDiscLauncher;
	

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float MaxHP{1000.0};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float CurrentHP{CurrentHP = MaxHP};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float BossSpeed{500.0f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float MeleeAttackDist{1000.0f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float PatrolDist{2000.0f};	
};
