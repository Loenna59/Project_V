// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAnimInstance.h"

#include "Boss/ThunderJaw.h"

void UThunderJawAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		FVector velocity = boss->GetVelocity();
		FVector forward = boss->GetActorForwardVector();

		BossAnimMoveSpeed = FVector::DotProduct(velocity,forward);
	}
}