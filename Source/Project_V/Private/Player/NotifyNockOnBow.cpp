// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NotifyNockOnBow.h"

#include "Player/PlayCharacter.h"

void UNotifyNockOnBow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	AActor* owner = MeshComp->GetOwner();
	if (owner)
	{
		if (APlayCharacter* player = Cast<APlayCharacter>(owner))
		{
			player->PlaceArrowOnBow();
		}
	}
}
