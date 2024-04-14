// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Kismet/GameplayStatics.h"

void ATurret::Fire()
{
	Super::Fire();
}

ATurret::ATurret() : Super()
{
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Mesh"));
	BarrelMesh->SetupAttachment(RotateMesh);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint1->SetupAttachment(BarrelMesh);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	// Shooting timer
	GetWorldTimerManager().SetTimer(ShootingTimer, this, &ATurret::Fire, FireRate, true, 0.f);

	RelativePickupSpawnPoint = FVector(0.0f, 0.0f, TurretPickupVerticalOffset);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);

	if (!Player)
	{
		return;
	}

	// FVector FromHereToPlayer = Player->GetActorLocation() - GetActorLocation();

	if (FromHereToPlayer.Length() > AgroRange || FVector::DotProduct(FromHereToPlayer, Player->GetActorForwardVector()) > 0.f)
	{
		return;
	}

	RotateMesh->SetWorldRotation(FRotator(0.f, FromHereToPlayer.Rotation().Yaw, 0.f));

	FVector FromBarrelToPlayer =  Player->GetActorLocation() - BarrelMesh->GetComponentLocation();

	BarrelMesh->SetRelativeRotation(FRotator(FromBarrelToPlayer.Rotation().Pitch, 0.f, 0.f));
}

