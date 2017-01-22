// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

UCLASS()
class ELITE_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocket();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void LaunchProjectile(FVector ShootDirection);

	/** update velocity on client */
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

private:
	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UProjectileMovementComponent* MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UParticleSystemComponent* ParticleComp;
	
};
