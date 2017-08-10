// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "EliteGameState.h"
#include "UnrealNetwork.h"
#include "EliteGameMode.h"
#include "MyPlayerController.h"

void AEliteGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AEliteGameState, Team1);
	DOREPLIFETIME(AEliteGameState, Team2);
	DOREPLIFETIME(AEliteGameState, Team1Score);
	DOREPLIFETIME(AEliteGameState, Team2Score);
	DOREPLIFETIME(AEliteGameState, Team1RoundScore);
	DOREPLIFETIME(AEliteGameState, Team2RoundScore);
}

void AEliteGameState::AddScoreToTeam(int PointsToAdd, int Team)
{
	if (Team == 1)
	{
		Team1Score += PointsToAdd;
	}
	else if (Team == 2)
	{
		Team2Score += PointsToAdd;
	}

	CheckIfRoundWon();
}

void AEliteGameState::CheckIfRoundWon()
{
	AEliteGameMode* GM = Cast<AEliteGameMode>(GetWorld()->GetAuthGameMode());
	if (Team1Score >= GM->RoundScoreLimit)
	{
		// Team 1 wins
		RoundWonByTeam(1);
		
	}
	else if (Team2Score >= GM->RoundScoreLimit)
	{
		// Team 2 Wins
		RoundWonByTeam(2);
	}
}

void AEliteGameState::RoundWonByTeam(int WinningTeam)
{
	if (WinningTeam == 1)
	{
		Team1RoundScore++;
		UE_LOG(LogTemp, Warning, TEXT("TEAM 1 WINS"));
	}
	else if (WinningTeam == 2)
	{
		Team2RoundScore++;
		UE_LOG(LogTemp, Warning, TEXT("TEAM 2 WINS"));
	}
	ResetRound();
}

void AEliteGameState::ResetRound()
{
	AEliteGameMode* GM = Cast<AEliteGameMode>(GetWorld()->GetAuthGameMode());
	Team1Score = 0;
	Team2Score = 0;
	GM->RespawnAllPlayers();
}

void AEliteGameState::BeginPlay()
{

}

// Called every frame
void AEliteGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*UE_LOG(LogTemp, Warning, TEXT("IN TICK"));
	for (APlayerState* State : PlayerArray)
	{
		AElitePlayerState* PS = Cast<AElitePlayerState>(State);
		UE_LOG(LogTemp, Warning, TEXT("%d"), PS->MyTeam);
	}*/
}

bool AEliteGameState::AddToTeam_Validate(int32 Team, AElitePlayerState* PlayerState)
{
	return true;
}

void AEliteGameState::AddToTeam_Implementation(int32 Team, AElitePlayerState* PlayerState)
{
	//AElitePlayerState* Player = Cast<AElitePlayerState>(PlayerState);
	if (Team == 1)
	{
		Team1.Add(PlayerState);
		UE_LOG(LogTemp, Warning, TEXT("TEAM 1 now has %d players"), Team1.Num());
	}
	else
	{
		Team2.Add(PlayerState);
		UE_LOG(LogTemp, Warning, TEXT("TEAM 2 now has %d players"), Team2.Num());
	}
}

void AEliteGameState::PrintTeams()
{
}