// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	void PostLogin(APlayerController* NewPlayer) override;

	TArray<APlayerController*> PlayerArray;

	virtual bool ReadyToStartMatch_Implementation() override;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugMode = false;

	UPROPERTY(EditDefaultsOnly, Category = "Round Config")
	int RoundScoreLimit = 2;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	TSubclassOf<class AFPSCharacter> CharacterBlueprint;

	void RespawnAllPlayers();
};
