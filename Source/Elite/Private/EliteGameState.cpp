// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "EliteGameState.h"
#include "UnrealNetwork.h"

void AEliteGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AEliteGameState, Team1);
	DOREPLIFETIME(AEliteGameState, Team2);
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