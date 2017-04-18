// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "EliteHUD.generated.h"

/**
 * 
 */
UCLASS()
class ELITE_API AEliteHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UFont* HUDFont;

	AEliteHUD(const FObjectInitializer& ObjectInitializer);
	
	/*primary draw call for the HUD*/
	virtual void DrawHUD() override;
	
	
};
