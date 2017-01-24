// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSCharacter.h"
#include "AttackerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ELITE_API AAttackerCharacter : public AFPSCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = Firing)
		float LaunchSpeed = 5000.f;

	UPROPERTY(EditAnywhere, Category = Setup)
		TSubclassOf<ARocket> RocketBlueprint;

public:
	// Sets default values for this character's properties
	AAttackerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
	void OnFire();
	
	void ChangeTeam();

	virtual void SetupMovementComponent() override;

	// GUN STUFF
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Ammo)
	int Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int MaxAmmo = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	float RechargeTime = 0.00001f;

	// Calls reload
	UFUNCTION(BlueprintCallable, Category=Firing)
	bool CanFire();

	UFUNCTION()
	void Reload();

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	UParticleSystem* RailBeam;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void CreateRailParticle(FVector Start, FVector End, FHitResult HitResult);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerNotifyShot(FHitResult HitResult, FVector Start, FVector End);

	/*UFUNCTION(Server, unreliable, WithValidation)
	void ServerFireRail(FVector Start, FVector End);*/

	void CheckIfHitEnemy(FHitResult HitResult);

	/** server notified of miss to show trail FX */
	/*UFUNCTION(unreliable, server, WithValidation)
	void ServerNotifyMiss(FVector ShootDir);*/
};
