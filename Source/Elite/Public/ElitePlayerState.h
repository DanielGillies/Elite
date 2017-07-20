// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "../Public/MyPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "ElitePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ELITE_API AElitePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	int Attacks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	int Rockets = 0;

	//AMyPlayerController MyController;

	UFUNCTION(BlueprintCallable, Category = "Team")
	void SetTeam(int32 Team);
	
	void BeginPlay();

	UPROPERTY(BlueprintReadWrite, Category = "Team")
	int32 MyTeam;
};
