// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Rocket.h"
#include "FPSCharacter.h"
#include "DefenderCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ELITE_API ADefenderCharacter : public AFPSCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Firing)
	float LaunchSpeed = 5000.f;
	
public:
	// Sets default values for this character's properties
	ADefenderCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<ARocket> RocketBlueprint;

	UFUNCTION()
	void OnFire();

	UPROPERTY(EditAnywhere, Category = Attributes)
	int Health = 3;

	void ChangeTeam();

	virtual void SetupMovementComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int MaxAmmo = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int Ammo;

	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Ammo)
	float RechargeTime = 1.5;

	bool CanFire();

	int GetCurrentAmmo();

	UFUNCTION(reliable, server, WithValidation)
	void ServerFireProjectile(FTransform ProjectileTransform);

	void Print(FString Message);
};
