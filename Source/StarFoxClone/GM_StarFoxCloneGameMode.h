// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_StarFoxCloneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STARFOXCLONE_API AGM_StarFoxCloneGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void ActorDied(AActor* DeadActor);
};
