// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShip.generated.h"

// Forward declarations
class UCapsuleComponent;
class ACameraActor;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
struct FTimerHandle;
class UHitFlash;
class UHealthComponent;
class AProjectile;
class UParticleSystem;
class UCameraShakeBase;
class ABombProjectile;

UCLASS()
class STARFOXCLONE_API APlayerShip : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* TriggerCapsuleComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	USceneComponent* LeftProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	USceneComponent* RightProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	USceneComponent* BombSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UHitFlash* HitFlashComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForwardMovementSpeed = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InputMovementMaxSpeed = 3500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InputMovementDeceleration = 85.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bIsMoving = false;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxMovementRotationAngle = 45.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MeshRollMovementScale = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxVerticalDistanceFromStart = 2300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxHorizontalDistanceFromStart = 4200.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector MoveDirection = FVector(0.f);

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector VelocityVector = FVector(0.f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSoftObjectPtr<ACameraActor> Camera;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraDistance = 1000.f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	FVector StartLocation = FVector(0.f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMovementScale = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float InterpSpeed = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	bool StartedCameraInterpToStart = false;

	FRotator CameraStartRotation = FRotator(0.f);
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotationScale = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> CollisionCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PlayerInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputFire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputBomb;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float CollisionOverlapKnockbackSpeed = 20000.f;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float KnockbackDeceleration = 100.f;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float CollisionOverlapDamage = 20.0f;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	bool bFreezeMovement = false;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float FreezeTime = 1.0f;

	FTimerHandle FreezeTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Collision")
	UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Collision")
	FVector DeathParticlesScale = FVector(5.f);

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	bool bIsInvincible = false;

	float InvincibilityTime = 3.0f;

	FTimerHandle InvincibleTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<ABombProjectile> BombClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 BombCount = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BombLaunchForwardImpulse = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.4f;

	bool bCanFire = true;

	bool bCanBomb = true;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BombTrajectoryPitchScaler = 0.5f;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* LaserSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* DeathSound;

public:
	// Sets default values for this pawn's properties
	APlayerShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input action bindings
	void Move(const FInputActionValue& Value);
	void StopMovement();
	void Fire(const FInputActionValue& Value);
	void ResetFireability();
	void Bomb(const FInputActionValue& Value);
	void ResetBombability();

	void UnFreezeMovement();
	void SetInvincibilityToFalse();
	void Die();

	void AddHealth(float HealthAmount);
	void AddBomb() { BombCount++; };

	bool GetIsInvincible() { return bIsInvincible; };

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
};
