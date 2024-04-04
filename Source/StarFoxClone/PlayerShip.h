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
	TSubclassOf<class UCameraShakeBase> CollisionCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputFire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PlayerInputMapping;

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
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Collision")
	FVector DeathParticlesScale = FVector(5.f);

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	bool bIsInvincible = false;

	float CollisionInvincibilityTime = 3.0f;

	FTimerHandle InvincibleTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.05f;

	bool bCanFire = true;

	FTimerHandle FireRateTimerHandle;

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

	void Move(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void StoppedMovement(const FInputActionValue& Value);
	void UnFreezeMovement();
	void SetInvincibilityToFalse();
	void ResetFireability();
	void Die();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
};
