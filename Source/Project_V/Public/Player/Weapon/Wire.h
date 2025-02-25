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

	void Link(class AActor* proj);

	bool IsChaining() const;
	
protected:
	virtual void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
