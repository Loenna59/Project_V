// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/ThunderJawAnimInstance.h"

#include "Boss/ThunderJaw.h"
#include "Components/AudioComponent.h"

void UThunderJawAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

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

	CurrentPlayingMontage = GetCurrentActiveMontage();
}

void UThunderJawAnimInstance::AnimNotify_PlayRageSound()
{
	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		boss->SoundInstance->SetSound(boss->RageSound);
		boss->SoundInstance->Play();
		boss->SoundInstance->SetVolumeMultiplier(1.0f);
	}
}

void UThunderJawAnimInstance::AnimNotify_PlayWalkSound()
{
	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		boss->SoundInstance->SetSound(boss->WalkSound);
		boss->SoundInstance->SetWorldLocation(boss->GetActorLocation());
		boss->SoundInstance->Play();
		boss->SoundInstance->SetVolumeMultiplier(0.75f);
	}
}

void UThunderJawAnimInstance::AnimNotify_PlayChargeSound()
{
	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		boss->SoundInstance->SetSound(boss->ChargeSound);
		boss->SoundInstance->SetWorldLocation(boss->GetActorLocation());
		boss->SoundInstance->Play();
		boss->SoundInstance->SetVolumeMultiplier(0.5f);
	}
}

void UThunderJawAnimInstance::AnimNotify_PlayFallDownSound()
{
	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		boss->SoundInstance->SetSound(boss->FallDownSound);
		boss->SoundInstance->SetWorldLocation(boss->GetActorLocation());
		boss->SoundInstance->Play();
		boss->SoundInstance->SetVolumeMultiplier(1.0f);
	}
}

void UThunderJawAnimInstance::AnimNotify_PlayDieSound()
{
	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		boss->SoundInstance->SetSound(boss->DieSound);
		boss->SoundInstance->SetWorldLocation(boss->GetActorLocation());
		boss->SoundInstance->Play();
		boss->SoundInstance->SetVolumeMultiplier(1.0f);
	}
}

void UThunderJawAnimInstance::AnimNotify_PlayTailSound()
{
	auto* boss = Cast<AThunderJaw>(TryGetPawnOwner());
	if (boss)
	{
		boss->SoundInstance->SetSound(boss->TailSound);
		boss->SoundInstance->SetWorldLocation(boss->GetActorLocation());
		boss->SoundInstance->Play();
		boss->SoundInstance->SetVolumeMultiplier(1.0f);
	}
}
