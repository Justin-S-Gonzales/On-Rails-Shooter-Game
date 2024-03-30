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

UCLASS()
class STARFOXCLONE_API APlayerShip : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	USceneComponent* LeftProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	USceneComponent* RightProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForwardMovementSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InputMovementMaxSpeed = 6000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InputMovementAcceleration = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InputMovementDeceleration = 200.f;

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

	FVector StartLocation = FVector(0.f);
	FVector StartDirection = FVector(0.f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraHeight = 0.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMovementScale = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraAngle = 0.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float InterpSpeed = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	bool StartedCameraInterpToStart = false;

	FRotator CameraStartRotation = FRotator(0.f);
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotationScale = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputFire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PlayerInputMapping;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float CollisionOverlapKnockbackSpeed = 20000.f;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float InstantKnockbackDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float KnockbackDeceleration = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	bool bFreezeMovement = false;

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
	void StoppedMovement(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
