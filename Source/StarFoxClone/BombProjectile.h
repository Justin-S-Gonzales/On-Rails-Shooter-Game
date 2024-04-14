// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombProjectile.generated.h"

class UCapsuleComponent;
class AExplosion;

UCLASS()
class STARFOXCLONE_API ABombProjectile : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* PhysicsCapsuleComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AExplosion> ExplosionClass;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* ExplosionSound;

public:	
	// Sets default values for this actor's properties
	ABombProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
