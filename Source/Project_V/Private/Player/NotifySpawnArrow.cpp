// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NotifySpawnArrow.h"

#include "Player/Component/PlayerCombat.h"

void UNotifySpawnArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	AActor* owner = MeshComp->GetOwner();
	if (owner)
	{
		if (UPlayerCombat* combatComp = Cast<UPlayerCombat>(owner->GetDefaultSubobjectByName("PlayerCombat")))
		{
			combatComp->SpawnArrow();
		}
	}
}
