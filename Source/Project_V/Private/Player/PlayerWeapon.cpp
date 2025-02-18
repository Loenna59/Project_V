// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerWeapon.h"

#include "Player/Arrow.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(mesh);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmp_bow(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Ranged/SKM_Bow.SKM_Bow'"));

	if (tmp_bow.Succeeded())
	{
		mesh->SetSkeletalMesh(tmp_bow.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tmp_weaponAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Bow.ABP_Bow_C'"));

	if (tmp_weaponAnim.Succeeded())
	{
		mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		mesh->SetAnimInstanceClass(tmp_weaponAnim.Class);
	}

	ConstructorHelpers::FClassFinder<AArrow> tmp_arrow(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Player/BP_Arrow.BP_Arrow_C'"));

	if (tmp_arrow.Succeeded())
	{
		arrowFactory = tmp_arrow.Class;
	}

	arrowSlot = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSlotComp"));
	arrowSlot->SetupAttachment(mesh, TEXT("bowstring"));
	arrowSlot->SetRelativeLocation(FVector(25, 0, 0));
	arrowSlot->SetRelativeScale3D(FVector(1.1f));

}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAnimInstance* APlayerWeapon::GetAnimInstance()
{
	if (mesh)
	{
		return mesh->GetAnimInstance();
	}

	return nullptr;
}

void APlayerWeapon::SpawnArrowInBow()
{
	if (arrow.IsValid())
	{
		return;
	}
	
	AArrow* spawned_arrow = GetWorld()->SpawnActor<AArrow>(arrowFactory);
	arrow = spawned_arrow;
	PlaceArrowOnBow();
}

void APlayerWeapon::SpawnArrow(USceneComponent* parent, FName socketName)
{
	if (arrow.IsValid())
	{
		return;
	}

	AArrow* spawned_arrow = GetWorld()->SpawnActor<AArrow>(arrowFactory);
	spawned_arrow->AttachToComponent(parent, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
	arrow = spawned_arrow;
}

void APlayerWeapon::PlaceArrowOnBow()
{
	if (arrow.IsValid())
	{
		arrow->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		arrow->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		arrow->AttachToComponent(arrowSlot, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void APlayerWeapon::PlaceOrSpawnArrow()
{
	if (arrow.IsValid())
	{
		PlaceArrowOnBow();
	}
	else
	{
		SpawnArrowInBow();
	}
}

bool APlayerWeapon::Fire(FVector direction, float alpha)
{
	if (arrow.IsValid())
	{
		arrow->Fire(direction, alpha);
		arrow = nullptr;

		return false;
	}

	return true;
}

void APlayerWeapon::AttachSocket(USceneComponent* comp, FName socketName, bool visibleArrow)
{
	AttachToComponent(comp, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);

	if (arrow.IsValid())
	{
		if (arrow->mesh)
		{
			arrow->mesh->SetVisibility(visibleArrow);
		}
	}
}

