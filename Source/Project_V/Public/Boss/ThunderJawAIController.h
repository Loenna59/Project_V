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
	UPROPERTY(EditAnywhere)
	float DetectRadius{1200.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DetectedTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceFromTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoseTargetTime{0.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FacingDot;
};
