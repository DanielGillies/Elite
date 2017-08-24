// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Rocket.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class ADefenderCharacter;
class AWeapon;

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

	UPROPERTY(EditAnywhere, Category = Movement)
	float WallJumpUpForce = .7f;

protected:
	UPROPERTY(EditAnywhere, Category = Attributes, Replicated)
		float Health = 100.0f;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float RespawnDelay = 2.0f;

public:


	/** WEAPONS **/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
	TSubclassOf<AWeapon> RailGunBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
	TSubclassOf<AWeapon> RocketWeaponBP;*/

	/** currently equipped weapon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons, Transient, Replicated)
	class AWeapon* CurrentWeapon;

	/** Default weapon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons, Replicated)
	TSubclassOf<AWeapon> DefaultWeapon;

	UFUNCTION(BlueprintCallable, Category = Weapons)
	void SetDefaultWeapon(TSubclassOf<AWeapon> Weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons", Server, Reliable, WithValidation)
	void SpawnWeapon(TSubclassOf<AWeapon> Weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons", Server, Reliable, WithValidation)
	void RemoveWeapon();

	void EquipWeapon(AWeapon* WeaponToEquip);

	/************/
	/** MESHES **/
	/************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	UMaterial* RedMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	UMaterial* BlueMat;

	//UPROPERTY(EditAnywhere, Category = Players)
	//TSubclassOf<ADefenderCharacter> DefenderBlueprint;

	// Sets default values for this character's properties
	AFPSCharacter();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

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

	UFUNCTION()
	void OnReady();

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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireProjectile(FTransform ProjectileTransform, AController* Shooter);

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
	void CreateRailParticle(FVector Start, FVector End, FHitResult HitResult, APlayerState* Shooter);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerNotifyShot(FHitResult HitResult, FVector Start, FVector End, APlayerState* Shooter);

	/*UFUNCTION(Server, unreliable, WithValidation)
	void ServerFireRail(FVector Start, FVector End);*/

	void CheckIfHitEnemy(FHitResult HitResult, APlayerState* Shooter);

	UFUNCTION(BlueprintCallable, Category=Info)
	float GetHealth() { return Health; };

	void SetHealth(float Health) { this->Health = Health; }

};
