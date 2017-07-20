// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "EliteGameState.h"
#include "MyPlayerController.h"
#include "ElitePlayerState.h"

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

void AEliteGameState::AddToTeam(int32 Team, AMyPlayerController* PC)
{
	//if (Team == 1)
	//{
	//	Team1.Add(PC);
	//}
	//else
	//{
	//	Team2.Add(PC);
	//}
}

void AEliteGameState::PrintTeams()
{
}