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

	UPROPERTY(BlueprintReadWrite, Category = "Team Scores", Replicated)
	int Team1Score = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Team Scores", Replicated)
	int Team2Score = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Team Scores", Replicated)
	int Team1RoundScore = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Team Scores", Replicated)
	int Team2RoundScore = 0;

	int AttackingTeam = 1;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void AddScoreToTeam(int PointsToAdd, int Team);

	void CheckIfRoundWon();

	void RoundWonByTeam(int WinningTeam);

	void ResetRound();
};
