// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAnimInstance.h"

#include "Boss/ThunderJaw.h"
#include "Boss/ThunderJawAIController.h"
#include "Player/PlayCharacter.h"

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

void UThunderJawAnimInstance::PlayMontage(UAnimMontage* montage)
{
	if (montage)
	{
		Montage_Play(montage);
		OnMontageBlendingOut.AddDynamic(this,&UThunderJawAnimInstance::OnAnimEnded);
	}
}

void UThunderJawAnimInstance::OnAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		boss->GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
	}
}
