// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThunderJawFSM.generated.h"

class UBossBaseState;
class AThunderJawAIController;

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle = 0,
	Patrol,
	Detect,
	Attack,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_V_API UThunderJawFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	UThunderJawFSM();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	void ChangeState(EBossState BossState);
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	UBossBaseState* GetCurrentState();
	UFUNCTION(BlueprintCallable, Category="ThunderJaw State")
	UBossBaseState* GetPrevState();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ThunderJaw State")
	class AThunderJawAIController* BossAIController;
protected:
	UFUNCTION()
	void InitStatePool();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ThunderJaw State")
	class AThunderJaw* Boss;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ThunderJaw State")
	UBossBaseState* CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ThunderJaw State")
	UBossBaseState* PrevState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThunderJaw State")
	TMap<EBossState, UBossBaseState*> StatePool;
};
