// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "EliteGameMode.h"
#include "public/EliteGameState.h"
#include "public/MyPlayerController.h"
#include "public/ElitePlayerState.h"
#include "public/ElitePlayerStart.h"
//
//AEliteGameMode::AEliteGameMode(const FObjectInitializer &ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	PlayerStateClass = AElitePlayerState::StaticClass();
//}


void AEliteGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerArray.Add(NewPlayer);

	//AEliteGameState* GS = Cast<AEliteGameState>(GetWorld()->GetGameState());
	//GS->Team1.Add(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("Num of players is now %d"), PlayerArray.Num());
}

/* CPP file of our GameMode Child Class */
bool AEliteGameMode::ReadyToStartMatch_Implementation() {
	//Super::ReadyToStartMatch();
	//return true;
	AEliteGameState* GS = Cast<AEliteGameState>(GameState);
	if (GS->Team1.Num() > 0 && GS->Team2.Num() > 0)
	{

		for (APlayerController* Controller : PlayerArray)
		{
			AElitePlayerState* PS = Cast<AElitePlayerState>(Controller->PlayerState);
			if (!PS->bIsReady)
			{
				return false;
			}
		}

		return true;
	}
	return false;
}
