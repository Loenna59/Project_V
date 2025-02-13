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
	void InitComponents();

public:
	UFUNCTION(BlueprintCallable)
	UThunderJawFSM* GetFSMComponent();
	UFUNCTION(BlueprintCallable)
	AThunderJawAIController* GetBossAIController();
	UFUNCTION(BlueprintCallable)
	APlayCharacter* GetAloy();

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	UThunderJawFSM* FSM;
	UPROPERTY()
	AThunderJawAIController* BossAIController;
	UPROPERTY()
	APlayCharacter* Aloy;


public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float DetectDist {600.0f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float MeleeAttackDist{300.0f};

	UPROPERTY()
	bool bIsRotateBody{false};
};
