// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NotifyStateLargeHit.h"

#include "Player/PlayCharacter.h"

void UNotifyStateLargeHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* owner = MeshComp->GetOwner();
	if (owner)
	{
		if (APlayCharacter* player = Cast<APlayCharacter>(owner))
		{
			player->bIsBlockControl = true;
		}
	}
}

void UNotifyStateLargeHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* owner = MeshComp->GetOwner();
	if (owner)
	{
		if (APlayCharacter* player = Cast<APlayCharacter>(owner))
		{
			player->bIsBlockControl = false;
		}
	}
}
