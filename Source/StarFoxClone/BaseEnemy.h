// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "BaseEnemy.generated.h"

class UParticleSystem;
class UHitFlash;
class UHealthComponent;
class UCapsuleComponent;
struct FTimerHandle;

UCLASS()
class STARFOXCLONE_API ABaseEnemy : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* TriggerCapsuleComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* RotateMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UHitFlash* HitFlashComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	USceneComponent* ProjectileSpawnPoint1;

	UPROPERTY(EditAnywhere, Category = "Death")
	UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Death")
	FVector DeathParticlesScale = FVector(5.f);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	FTimerHandle ShootingTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AgroRange = 20000.f;

	FVector FromHereToPlayer = FVector(0.f);

	virtual void Fire();

public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Die();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

};
