// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Turret.generated.h"

/**
 * 
 */
UCLASS(CustomConstructor)
class STARFOXCLONE_API ATurret : public ABaseEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* BarrelMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components");
	UStaticMeshComponent* BaseMesh;

	void Fire();
public:
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
