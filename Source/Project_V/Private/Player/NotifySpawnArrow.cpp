// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NotifySpawnArrow.h"

#include "Project_V.h"
#include "Player/PlayCharacter.h"

void UNotifySpawnArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	AActor* owner = MeshComp->GetOwner();
	if (owner)
	{
		if (APlayCharacter* player = Cast<APlayCharacter>(owner))
		{
			player->SpawnArrow();
		}
	}
}
