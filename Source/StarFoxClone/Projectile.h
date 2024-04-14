// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UCapsuleComponent;
class UParticleSystem;

UCLASS()
class STARFOXCLONE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* TriggerCapsuleComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 30000.0f;

	FVector StartLocation = FVector(0.f);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FVector ImpactParticlesScale = FVector(3.0f);

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 10.0f;

public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
