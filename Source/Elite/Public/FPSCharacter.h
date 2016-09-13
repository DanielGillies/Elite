// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Rocket.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class ELITE_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Movement)
	float WallCheckRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float WallJumpForce = 500.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float JumpForce = 300.f;

	UPROPERTY(EditAnywhere, Category = Firing)
	float LaunchSpeed = 5000.f;

	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<ARocket> RocketBlueprint;
public:
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

	// Walk speed multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomMovement)
	float WalkSpeed = 800;

	// Sprint speed multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomMovement)
	float SprintSpeed = 1500;

	// Sprinting Flag
	bool bSprintActive = false;

	// WallJump Flag
	bool bCanWallJump = false;

	bool CheckForWalls(FVector& OutImpactVector);

	bool CheckLeftForWalls(FHitResult& OutHitResult);

	bool CheckRightForWalls(FHitResult& OutHitResult);

};
