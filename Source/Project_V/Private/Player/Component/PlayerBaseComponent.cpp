// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/PlayerBaseComponent.h"
#include "Player/PlayCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Player/PlayerAnimInstance.h"


// Sets default values for this component's properties
UPlayerBaseComponent::UPlayerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<APlayCharacter>(GetOwner());
	anim = Cast<UPlayerAnimInstance>(me->GetMesh()->GetAnimInstance());
}

