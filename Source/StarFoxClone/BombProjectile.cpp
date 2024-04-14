// Fill out your copyright notice in the Description page of Project Settings.


#include "BombProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Explosion.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABombProjectile::ABombProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	PhysicsCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Physics Capsule Comp"));
	PhysicsCapsuleComp->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ABombProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	PhysicsCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABombProjectile::OnOverlapBegin);
}

// Called every frame
void ABombProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABombProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Bomb Overlap Event"));

	if (IsActorBeingDestroyed())
	{
		return;
	}

	if (Cast<AExplosion>(OtherActor))
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	AExplosion* Explosion = GetWorld()->SpawnActor<AExplosion>(ExplosionClass, GetActorLocation()/* + FVector::UpVector * 100.f*/, GetActorRotation());

	Destroy();
}

