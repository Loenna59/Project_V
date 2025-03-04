// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapon.h"
#include "Katana.generated.h"

UCLASS()
class PROJECT_V_API AKatana : public APlayerWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKatana();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	bool isAttacking = false;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* mesh;
	
	void StartTraceChannel();

	void EndTraceChannel();
};
