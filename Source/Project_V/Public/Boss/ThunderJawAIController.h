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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MoveToPlayer();
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& Actors);
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UAIPerceptionComponent* AIPC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Sence")
	class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Sence")
	class UAISenseConfig_Hearing* HearingConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Sence")
	class UAISenseConfig_Damage* DamageConfig;

	UPROPERTY()
	float OnDetected{false};
	
};
