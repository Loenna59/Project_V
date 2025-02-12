// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossAnim.h"

#include "Boss/ThunderJaw.h"

void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
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
