// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NotifyPutBow.h"

#include "Player/PlayCharacter.h"

void UNotifyPutBow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	if (AActor* owner = MeshComp->GetOwner())
	{
		if (APlayCharacter* player = Cast<APlayCharacter>(owner))
		{
			player->PickWeapon();
		}
	}
}
