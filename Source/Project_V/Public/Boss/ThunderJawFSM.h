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
	UFUNCTION()
	void InitPatrolPoints();

public:
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	void ChangeBossState(EBossState BossState);
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	UBossBaseState* GetCurrentState();
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	UBossBaseState* GetPrevState();
	UFUNCTION()
	void ChangePatrolTargetPoint();
	UFUNCTION()
	void AdjustSpeed(float NewSpeed);
	
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
	TArray<FVector> PatrolPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol")
	int CurrentTargetPoint{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol")
	bool ArrivedTargetPoint{false};
};
