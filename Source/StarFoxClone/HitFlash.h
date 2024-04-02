// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitFlash.generated.h"

struct FTimerHandle;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARFOXCLONE_API UHitFlash : public UActorComponent
{
	GENERATED_BODY()

private:
	AActor* Owner;
	TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;

	UPROPERTY(EditAnywhere, Category = "Emission")
	FColor FlashColor = FColor(0.f);

	UPROPERTY(EditAnywhere, Category = "Emission")
	float FlashIntensity = 1.f;

	UPROPERTY(EditAnywhere, Category = "Emission")
	FVector4 ResetEmissiveColor = FVector4(0.f);

	UPROPERTY(EditAnywhere, Category = "Emission")
	float ResetIntensity = 0.f;

	FTimerHandle FlashTimerHandle;
	FTimerHandle ClearFlashTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Flash")
	int32 NumFlashes = 5;

	int32 CurrentNumFlashesLeft = NumFlashes;

	UPROPERTY(EditAnywhere, Category = "Flash")
	float FlashTime = 0.3f;

public:	
	// Sets default values for this component's properties
	UHitFlash();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void DamageFlash(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Flash();

	void ResetFlash();
};
