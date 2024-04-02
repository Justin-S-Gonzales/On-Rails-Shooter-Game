// Fill out your copyright notice in the Description page of Project Settings.


#include "HitFlash.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UHitFlash::UHitFlash()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHitFlash::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (Owner != nullptr)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHitFlash::DamageFlash);
	}

	UStaticMeshComponent* Mesh = Owner->GetComponentByClass <UStaticMeshComponent>();

	if (Mesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Mesh!"));
		return;
	}

	for (int i = 0; i < Mesh->GetMaterials().Num(); i++)
	{
		DynamicMaterialInstances.Add(Mesh->CreateDynamicMaterialInstance(i));
	}
}

void UHitFlash::DamageFlash(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Took Damage in Hit Flash"));

	CurrentNumFlashesLeft = NumFlashes;

	Flash();
}

void UHitFlash::Flash()
{
	for (auto i : DynamicMaterialInstances)
	{
		i->SetVectorParameterValue(TEXT("Emissive Color"), FlashColor);
		i->SetScalarParameterValue(TEXT("Emission Multiplier"), FlashIntensity);
	}

	// Set timer for flash
	Owner->GetWorldTimerManager().SetTimer(ClearFlashTimerHandle, this, &UHitFlash::ResetFlash, FlashTime / 2.0f, false, FlashTime / 2.0f);
}

void UHitFlash::ResetFlash()
{
	CurrentNumFlashesLeft--;

	for (auto i : DynamicMaterialInstances)
	{
		i->SetVectorParameterValue(TEXT("Emissive Color"), ResetEmissiveColor);
		i->SetScalarParameterValue(TEXT("Emission Multiplier"), ResetIntensity);
	}

	// If no more flashes, we stop
	if (CurrentNumFlashesLeft <= 0)
	{
		return;
	}

	// Set timer for flash
	Owner->GetWorldTimerManager().SetTimer(FlashTimerHandle, this, &UHitFlash::Flash, FlashTime / 2.0f, false, FlashTime / 2.0f);
}

