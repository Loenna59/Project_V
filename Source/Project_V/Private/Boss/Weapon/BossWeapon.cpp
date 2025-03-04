// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/BossWeapon.h"

#include "Project_V.h"
#include "Boss/ThunderJaw.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABossWeapon::ABossWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("'/Game/Blueprints/Boss/Material/M_WeakParts.M_WeakParts'"));
	if (tempMat.Succeeded())
	{
		WeakPartsMatInst = tempMat.Object;
	}
}


// Called when the game starts or when spawned
void ABossWeapon::BeginPlay()
{
	Super::BeginPlay();
	Boss = Cast<AThunderJaw>(UGameplayStatics::GetActorOfClass(GetWorld(),AThunderJaw::StaticClass()));
}

// Called every frame
void ABossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossWeapon::DamageWeaponHP(float Damage)
{
	CurrentHP -= Damage;
	if (CurrentHP <= 0)
	{
		bIsBroken = true;
	}
}

void ABossWeapon::CheckFocusModeAndChangeMat(bool focusMode)
{
	if (focusMode)
	{
		Mesh->SetOverlayMaterial(WeakPartsMatInst);
	}
	else
	{
		if (!GetWorld()->GetTimerManager().TimerExists(ChangeWeakPartsMatTimerHandle))
		{
			PRINT_CALLINFO();
			GetWorld()->GetTimerManager().SetTimer(ChangeWeakPartsMatTimerHandle,[this]()
			{
				Mesh->SetOverlayMaterial(nullptr);
			},5,false);
		}
	}
}

