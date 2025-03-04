// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NotifyStateKatana.h"

#include "Player/Component/PlayerCombat.h"

void UNotifyStateKatana::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* owner = MeshComp->GetOwner();
	if (owner)
	{
		if (UPlayerCombat* combatComp = Cast<UPlayerCombat>(owner->GetDefaultSubobjectByName("PlayerCombat")))
		{
			combatComp->OnStartTraceKatanaChannel();
		}
	}
}

void UNotifyStateKatana::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* owner = MeshComp->GetOwner();
	if (owner)
	{
		if (UPlayerCombat* combatComp = Cast<UPlayerCombat>(owner->GetDefaultSubobjectByName("PlayerCombat")))
		{
			combatComp->OnEndTraceKatanaChannel();
		}
	}
}
