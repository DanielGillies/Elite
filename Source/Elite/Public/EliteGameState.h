// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "EliteGameState.generated.h"

class AMyPlayerController;

/**
 * 
 */
UCLASS()
class ELITE_API AEliteGameState : public AGameState
{
	GENERATED_BODY()
public:
	void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void AddToTeam(int32 Team, AMyPlayerController* PC);
	
	void PrintTeams();

	TArray<AMyPlayerController*> Team1;

	TArray<AMyPlayerController*> Team2;
};
