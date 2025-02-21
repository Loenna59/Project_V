// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "ThunderJawAIController.generated.h"

UENUM(BlueprintType)
enum class EBossSence : uint8
{
	None = 0,
	Sight,
	Hearing,
	Damage,
};
UCLASS()
class PROJECT_V_API AThunderJawAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AThunderJawAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
	UFUNCTION()
	void InitComponent();
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void UpdateDistanceFromTarget();
	UFUNCTION()
	void UpdateFacingDot();
	UFUNCTION()
	void EvaluateTargetDistance(float DeltaTime);
	UFUNCTION()
	void CheckTargetThroughStimulus();

public:

	UFUNCTION()
	void MoveToPlayer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class AThunderJaw* Boss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UAIPerceptionComponent* AIPC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Sence")
	class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Sence")
	class UAISenseConfig_Hearing* HearingConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Sence")
	class UAISenseConfig_Damage* DamageConfig;


public:
	// target이 sight안의 LookOut 경계에 들어옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	bool DetectedTarget{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	FVector DetectedLocation;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float DistanceFromTarget{0.0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float DetectDist{3600.0};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float CombatDist {1800.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float LoseTargetTime{0.0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float FacingDot{0.0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	bool bDebugMode{false};	
};
