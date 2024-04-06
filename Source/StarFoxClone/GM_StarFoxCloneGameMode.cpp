// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_StarFoxCloneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerShip.h"
#include "BaseEnemy.h"

void AGM_StarFoxCloneGameMode::ActorDied(AActor* DeadActor)
{
	if (APlayerShip* Player = Cast<APlayerShip>(DeadActor))
	{
		Player->Die();
	}

	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(DeadActor))
	{
		Enemy->Die();
	}
}
