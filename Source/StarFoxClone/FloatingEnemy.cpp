// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingEnemy.h"
#include "Kismet/GameplayStatics.h"

void AFloatingEnemy::Fire()
{
	Super::Fire();

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint2->GetComponentLocation(), ProjectileSpawnPoint2->GetComponentRotation());
}

AFloatingEnemy::AFloatingEnemy() : Super()
{
	ProjectileSpawnPoint1->SetupAttachment(RotateMesh);

	ProjectileSpawnPoint2 = CreateDefaultSubobject<USceneComponent>(
		TEXT("Projectile Spawn Point 2"));
	ProjectileSpawnPoint2->SetupAttachment(RotateMesh);
}

void AFloatingEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Shooting timer
	GetWorldTimerManager().SetTimer(ShootingTimer, this, &AFloatingEnemy::Fire, FireRate, true, 0.f);
}

void AFloatingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!UGameplayStatics::GetPlayerPawn(this, 0))
	{
		return;
	}

	FVector FromPlayerToHere = GetActorLocation() - UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();

	if (FromHereToPlayer.Length() > AgroRange || FVector::DotProduct(FromPlayerToHere, UGameplayStatics::GetPlayerPawn(this, 0)->GetActorForwardVector()) < 0.f)
	{
		return;
	}

	FVector VelocityVector = FromHereToPlayer.GetSafeNormal() * MovementSpeed;

	SetActorLocation(GetActorLocation() + VelocityVector * DeltaTime);

	SetActorRotation(FromHereToPlayer.Rotation());
}
