// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

class UCapsuleComponent;

UCLASS()
class STARFOXCLONE_API AExplosion : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* TriggerCapsuleComp;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float StartScale = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float GrowthRate = 15.f;

	float CurrentScale = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float MaxScale = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	float StartCollisionScale = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DamageRate = 1.0f;


public:	
	// Sets default values for this actor's properties
	AExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DamageOtherActor(AActor* OtherActor);
};
