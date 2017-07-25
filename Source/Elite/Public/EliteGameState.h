// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "EliteGameState.generated.h"

// Forward Declarations
//class AMyPlayerController;
class AElitePlayerState;

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

	UFUNCTION(BlueprintCallable, Category = "Team", Server, Reliable, WithValidation)
	void AddToTeam(int32 Team, AElitePlayerState* PlayerState);
	
	void PrintTeams();

	UPROPERTY(BlueprintReadWrite, Category = "GameState Team", Replicated)
	TArray<AElitePlayerState*> Team1;

	UPROPERTY(BlueprintReadWrite, Category = "GameState Team", Replicated)
	TArray<AElitePlayerState*> Team2;

	int AttackingTeam = 1;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
