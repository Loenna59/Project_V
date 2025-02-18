// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/NotifyEndDodge.h"
#include "Player/PlayCharacter.h"

void UNotifyEndDodge::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	if (AActor* owner = MeshComp->GetOwner())
	{
		if (APlayCharacter* player = Cast<APlayCharacter>(owner))
		{
			player->SetPlayingDodge(false);
		}
	}
}
