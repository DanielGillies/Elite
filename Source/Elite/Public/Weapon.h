// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		ERail			UMETA(DisplayName = "Rail"),
		EProjectile		UMETA(DisplayName = "Projectile"),
	};
}

USTRUCT()
struct FWeaponData
{
	//GENERATED_USTRUCT_BODY();
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 ShotCost;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		float Damage;
};

UCLASS()
class ELITE_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerNotifyShot(FVector Start, FVector End, FHitResult HitResult);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void DrawRailParticle(FVector Start, FVector End);


public:
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void InstantFire();

	UFUNCTION()
	void FireProjectile();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireProjectile(FTransform ProjectileTransform);

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	// Particle
	UPROPERTY(EditDefaultsOnly, Category = RailInfo)
	UParticleSystem* RailBeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	USkeletalMeshComponent* WeaponMesh;

	// Projectile
	UPROPERTY(EditDefaultsOnly, Category = Config)
	TSubclassOf<class ARocket> ProjectileBlueprint;
	
};
