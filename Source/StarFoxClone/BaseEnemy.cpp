// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "HitFlash.h"
#include "HealthComponent.h"
#include "Projectile.h"

void ABaseEnemy::Fire()
{
	// Spawn lasers
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint1->GetComponentLocation(), ProjectileSpawnPoint1->GetComponentRotation());
}

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule Comp"));
	TriggerCapsuleComp->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerCapsuleComp;

	RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotate Mesh"));
	RotateMesh->SetupAttachment(TriggerCapsuleComp);

	ProjectileSpawnPoint1 = CreateDefaultSubobject<USceneComponent>(
		TEXT("Projectile Spawn Point 1"));

	HitFlashComp = CreateDefaultSubobject<UHitFlash>(TEXT("Hit Flash"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!UGameplayStatics::GetPlayerPawn(this, 0))
	{
		return;
	}

	FromHereToPlayer = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation() - GetActorLocation();

	if (FromHereToPlayer.Length() > AgroRange || FVector::DotProduct(FromHereToPlayer, UGameplayStatics::GetPlayerPawn(this, 0)->GetActorForwardVector()) > 0.f)
	{
		GetWorldTimerManager().PauseTimer(ShootingTimer);
		return;
	}

	GetWorldTimerManager().UnPauseTimer(ShootingTimer);
}

void ABaseEnemy::Die()
{
	// UE_LOG(LogTemp, Warning, TEXT("died"));

	if (!DeathParticles)
	{
		UE_LOG(LogTemp, Warning, TEXT("No death particles!"));
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(
		this,
		DeathParticles,
		GetActorLocation(),
		GetActorRotation(),
		DeathParticlesScale
	);

	Destroy();
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// UE_LOG(LogTemp, Warning, TEXT("Took Damage in Base Enemy"));

	return 0.0f;
}

