// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "EliteGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ELITE_API AEliteGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	void PostLogin(APlayerController* NewPlayer) override;

	TArray<APlayerController*> PlayerArray;

	virtual bool ReadyToStartMatch_Implementation() override;
};
