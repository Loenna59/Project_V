// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapon/Wire.h"
#include "CableComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AWire::AWire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cableComp = CreateDefaultSubobject<UCableComponent>(TEXT("CableComp"));
	cableComp->SetupAttachment(mesh);

	cableComp->SetRelativeLocation(FVector(-38, 0, 0));
	cableComp->bAttachEnd = false;
	cableComp->CableWidth = 3;
	cableComp->SetVisibility(false);
	cableComp->SetGenerateOverlapEvents(false);

	cableCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CableCollision"));
	cableCollision->SetupAttachment(cableComp);
	cableCollision->SetBoxExtent(FVector(5));
	cableCollision->SetCollisionProfileName("PlayerProjectile");
	cableCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UParticleSystem> temp_fx(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/GoodFXImpact/FX/Particle/Chest_Point/Blue/PS_GFXI_Blue_Lightning_02.PS_GFXI_Blue_Lightning_02'"));

	if (temp_fx.Succeeded())
	{
		fx = temp_fx.Object;
	}
	
}

void AWire::BeginPlay()
{
	Super::BeginPlay();

	cableCollision->SetGenerateOverlapEvents(false);
	cableCollision->OnComponentHit.AddDynamic(this, &AWire::OnCableComponentHit);
}

void AWire::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void AWire::OnCableComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
}

void AWire::DestroyAfterPlayFX()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fx, GetActorLocation(), FRotator::ZeroRotator, FVector(10));
	Destroy();
}

void AWire::Link(AActor* proj)
{
	cableComp->SetAttachEndTo(proj, NAME_None);
	
	if (proj)
	{
		cableComp->bAttachEnd = true;
		cableComp->EndLocation = FVector(-38, 0, 0);
		cableComp->SetVisibility(true);
	}
	else
	{
		cableComp->bAttachEnd = false;
		cableComp->SetVisibility(false);
	}
}

bool AWire::IsChaining() const
{
	return cableComp->GetAttachedActor() != nullptr;
}

float AWire::GetLength() const
{
	TArray<FVector> locations;
	cableComp->GetCableParticleLocations(locations);

	return FVector::Distance(locations.Last(), locations[0]);
}

void AWire::SetEnableCollision()
{
	if (cableComp->bAttachEnd && IsChaining())
	{
		FVector dist = cableComp->GetAttachedActor()->GetActorLocation() - GetActorLocation();

		FVector center = FMath::Lerp(GetActorLocation(), cableComp->GetAttachedActor()->GetActorLocation(), 0.5f);
		FRotator rotation = FRotationMatrix::MakeFromY(dist).Rotator();
		
		cableCollision->SetBoxExtent(FVector(20, dist.Length() * 0.5f, 20));
		cableCollision->SetWorldLocation(center);
		cableCollision->SetWorldRotation(rotation);
		cableCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		cableCollision->SetGenerateOverlapEvents(true);
	}
}

