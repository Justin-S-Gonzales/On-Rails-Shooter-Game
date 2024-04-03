// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_StarFoxCloneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerShip.h"

void AGM_StarFoxCloneGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		Cast<APlayerShip>(DeadActor)->Die();
	}
}
