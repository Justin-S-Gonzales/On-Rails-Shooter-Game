// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GM_StarFoxCloneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerShip.h"

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// UE_LOG(LogTemp, Warning, TEXT("Took Damage in Health Component"));

	/*if (APlayerShip* Player = Cast<APlayerShip>(DamagedActor))
	{
		if (Player->GetIsInvincible())
		{
			UE_LOG(LogTemp, Warning, TEXT("Is Invincible"));
			return;
		}
	}*/

	CurrentHealth -= Damage;

	if (CurrentHealth <= 0.f)
	{
		AGM_StarFoxCloneGameMode* StarFoxCloneGameMode = Cast<AGM_StarFoxCloneGameMode>(UGameplayStatics::GetGameMode(this));
		
		if (!StarFoxCloneGameMode)
			return;

		StarFoxCloneGameMode->ActorDied(GetOwner());

		return;
	}

	if (Cast<APlayerShip>(DamagedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("%f health"), CurrentHealth);
	}
}

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::AddHealth(float HealthAmount)
{
	CurrentHealth = FMath::Min(CurrentHealth + HealthAmount, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("%f health"), CurrentHealth);
}

