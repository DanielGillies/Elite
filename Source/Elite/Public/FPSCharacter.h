// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Rocket.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class ADefenderCharacter;

UCLASS()
class ELITE_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Movement)
	float StrafeSpeedMult = 1.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float WallCheckRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float WallJumpForce = 500.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float JumpForce = 350.f;

protected:
	UPROPERTY(EditAnywhere, Category = Attributes)
	int Health = 3;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float RespawnDelay = 4.0f;

	void Die(class AMyPlayerController* PC);

public:

	//UPROPERTY(EditAnywhere, Category = Players)
	//TSubclassOf<ADefenderCharacter> DefenderBlueprint;

	// Sets default values for this character's properties
	AFPSCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	// Handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Value);

	// Handles Strafing
	UFUNCTION()
	void MoveRight(float Value);

	// Sets jump flag when key is pressed
	UFUNCTION()
	void OnStartJump();

	// Clears jump flag when key is released
	UFUNCTION()
	void OnStopJump();

	// Custom jump
	UFUNCTION()
	void Jump();
	
	UFUNCTION()
	void JumpReleased();

	UFUNCTION()
	void OnFire();

	/*UFUNCTION()
	void OnFire();*/

	// Walk speed multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomMovement)
	float WalkSpeed = 600;

	// Sprint speed multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomMovement)
	float SprintSpeed = 1000;

	// Sprinting Flag
	bool bSprintActive = false;

	// WallJump Flag
	bool bCanWallJump = false;

	bool CheckForWalls(FVector& OutImpactVector);

	bool CheckLeftForWalls(FHitResult& OutHitResult);

	bool CheckRightForWalls(FHitResult& OutHitResult);

	void ChangeTeam();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void SetupMovementComponent();

	UFUNCTION(unreliable, server, WithValidation)
	void ServerRequestSprint(bool bSprintActive);

	UFUNCTION(unreliable, server, WithValidation)
	void ServerRequestWalljump(FVector WallImpact);

	UFUNCTION(unreliable, server, WithValidation)
	void ServerRequestJump();


	/* Rail gun stuff */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RailInfo)
	int RailAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RailInfo)
	int MaxAmmo = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RailInfo)
	float RechargeTime = 0.00001f;

	UFUNCTION()
	void FireRail();

	UFUNCTION()
	void FireRocket();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void ServerFireProjectile(FTransform ProjectileTransform);

	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<ARocket> RocketBlueprint;

	// Calls reload
	UFUNCTION(BlueprintCallable, Category = RailInfo)
	bool CanFire();

	UFUNCTION()
	void Reload();

	UPROPERTY(EditDefaultsOnly, Category = RailInfo)
	UParticleSystem* RailBeam;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void CreateRailParticle(FVector Start, FVector End, FHitResult HitResult);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerNotifyShot(FHitResult HitResult, FVector Start, FVector End);

	/*UFUNCTION(Server, unreliable, WithValidation)
	void ServerFireRail(FVector Start, FVector End);*/

	void CheckIfHitEnemy(FHitResult HitResult);

};
