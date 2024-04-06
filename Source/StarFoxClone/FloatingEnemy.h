// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "FloatingEnemy.generated.h"

class UCapsuleComponent;

UCLASS(CustomConstructor)
class STARFOXCLONE_API AFloatingEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	USceneComponent* ProjectileSpawnPoint2;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float MovementSpeed = 1000.f;

	void Fire() override;

public:
	AFloatingEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
