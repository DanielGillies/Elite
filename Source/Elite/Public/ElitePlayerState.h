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

	UPROPERTY(BlueprintReadWrite, Category = "Ready", Replicated)
	bool bIsReady = false;

	UFUNCTION(BlueprintCallable, Category = "Team", Server, Reliable, WithValidation)
	void SetTeam(int32 Team, APlayerState* PlayerState);

	UFUNCTION(BlueprintCallable, Category = "Team")
	int32 GetTeam();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void BeginPlay();

	UPROPERTY(BlueprintReadWrite, Category = "Team", Replicated)
	int32 MyTeam;

	UFUNCTION(BlueprintCallable, Category = "Ready", Server, Unreliable, WithValidation)
	void ToggleReady(AElitePlayerState* PS);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", Replicated)
	TSubclassOf<class AWeapon> EquippedWeapon;
};
