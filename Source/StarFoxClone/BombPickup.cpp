// Fill out your copyright notice in the Description page of Project Settings.


#include "BombPickup.h"
#include "Components/CapsuleComponent.h"
#include "PlayerShip.h"
#include "Kismet/GameplayStatics.h"

void ABombPickup::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABombPickup::OnBeginOverlap);
}

void ABombPickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		Cast<APlayerShip>(OtherActor)->AddBomb();
	}

	Super::OnBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
