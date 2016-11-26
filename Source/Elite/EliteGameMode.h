// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/AttackerCharacter.h"
#include "Public/DefenderCharacter.h"
#include "GameFramework/GameMode.h"
#include "EliteGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ELITE_API AEliteGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Players)
	TSubclassOf<ADefenderCharacter> DefenderBlueprint;

	UPROPERTY(EditAnywhere, Category = Players)
	TSubclassOf<AAttackerCharacter> AttackerBlueprint;
};
