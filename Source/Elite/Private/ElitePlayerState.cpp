// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "ElitePlayerState.h"
#include "UnrealNetwork.h"
#include "../Public/EliteGameState.h"

void AElitePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AElitePlayerState, MyTeam);
}

void AElitePlayerState::BeginPlay()
{
	//UE_LOG(LogTemp, Warning, TEXT("PLAYERSTATE"));

}

//void AElitePlayerState::SetTeam(int32 Team)
//{
//	MyTeam = Team;
//	//AEliteGameState* GS = Cast<AEliteGameState>(GetWorld()->GetGameState())
//	//GetWorld()->GetGameState()->
//}

bool AElitePlayerState::SetTeam_Validate(int32 Team, APlayerState* PlayerState)
{
	return true;
}


void AElitePlayerState::SetTeam_Implementation(int32 Team, APlayerState* PlayerState)
{
	/*FString netmode = "";
	if (GetNetMode() == NM_Client)
		netmode = "CLIENT";
	else if (GetNetMode() == NM_ListenServer)
		netmode = "LISTEN";
	else if (GetNetMode() == NM_DedicatedServer)
		netmode = "SERVER";

	UE_LOG(LogTemp, Warning, TEXT("%s"), *netmode);*/
	MyTeam = Team;

	AElitePlayerState* PS = Cast<AElitePlayerState>(PlayerState);
	AEliteGameState* GS = Cast<AEliteGameState>(GetWorld()->GetGameState());
	GS->AddToTeam(Team, PS);
}

int32 AElitePlayerState::GetTeam()
{
	return MyTeam;
}

