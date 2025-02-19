// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThunderJawFSM.generated.h"

class UBossBaseState;

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle = 0,
	Patrol,
	Combat,
	LookOut,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UThunderJawFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	UThunderJawFSM();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void InitStatePool();

public:
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	void ChangeBossState(EBossState BossState);
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	UBossBaseState* GetCurrentState();
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	UBossBaseState* GetPrevState();
	UFUNCTION()
	bool GetRandomLocationFromNavMesh(FVector CenterLocation, float Radius, FVector& Dest);

	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ThunderJaw State")
	class AThunderJaw* Boss;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ThunderJaw State")
	UBossBaseState* CurrentState;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ThunderJaw State")
	UBossBaseState* PrevState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw State")
	TMap<EBossState, UBossBaseState*> StatePool;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol")
	FVector RandomLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol")
	bool bIsArriveDestLoc{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol")
	bool bIsRotateEnd{false};
};
