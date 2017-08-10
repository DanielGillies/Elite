// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSCharacter.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerTeam)
	int MyTeam;

	bool Attacking;

	void BeginPlay();

	void Tick(float DeltaTime);

	AFPSCharacter* GetControlledPlayer() const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	bool GetSightRayHitLocation(FVector& HitLocation, float WeaponRange) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation, float WeaponRange) const;
	
	FTimerHandle RespawnTimerHandle;

	void Die();

	UFUNCTION(reliable, server, WithValidation)
	void ServerRespawn();

	void Respawn();

	void SetTeam(int Team);

private:
	UPROPERTY(EditAnywhere)
	float CrosshairXLocation = 0.5f;

	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = 0.5f;
};
