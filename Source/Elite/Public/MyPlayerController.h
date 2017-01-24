// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "FPSCharacter.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class ELITE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	bool Attacking;

	void BeginPlay();

	void Tick(float DeltaTime);

	AFPSCharacter* GetControlledPlayer() const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	bool GetSightRayHitLocation(FVector& HitLocation) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	UFUNCTION(reliable, server, WithValidation)
	void ChangeTeam(AFPSCharacter* Caller);
	
	void Die(AFPSCharacter* Caller);

	UFUNCTION(reliable, server, WithValidation)
	void ServerRespawn();

	void Respawn();
};
