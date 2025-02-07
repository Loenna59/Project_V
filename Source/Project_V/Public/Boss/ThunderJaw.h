// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThunderJaw.generated.h"

UCLASS()
class PROJECT_V_API AThunderJaw : public ACharacter
{
	GENERATED_BODY()

public:
	AThunderJaw();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	void InitComponents();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw Character")
	class UThunderJawFSM* FSM;
};
