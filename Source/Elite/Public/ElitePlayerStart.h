// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "ElitePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class ELITE_API AElitePlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Defaults")
		bool bTeamB;
	
	
};
