// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/BossWeapon.h"


// Sets default values
ABossWeapon::ABossWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ABossWeapon::BeginPlay()
{
	Super::BeginPlay();
	
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
