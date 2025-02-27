// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerProjectile.h"
#include "Wire.generated.h"

UCLASS()
class PROJECT_V_API AWire : public APlayerProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWire();

	UPROPERTY(VisibleAnywhere)
	class UCableComponent* cableComp;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* cableCollision;

	void Link(class AActor* proj);

	bool IsChaining() const;

	float GetLength() const;

	UFUNCTION()
	void SetEnableCollision();

	virtual void DestroyAfterPlayFX() override;
protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	class UParticleSystem* fx;
	
	UFUNCTION()
	void OnCableComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
