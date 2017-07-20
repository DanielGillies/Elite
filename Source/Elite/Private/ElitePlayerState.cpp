// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "ElitePlayerState.h"
#include "UnrealNetwork.h"

void AElitePlayerState::BeginPlay()
{
	//UE_LOG(LogTemp, Warning, TEXT("PLAYERSTATE"));

}

void AElitePlayerState::SetTeam(int32 Team)
{
	MyTeam = Team;
	
}