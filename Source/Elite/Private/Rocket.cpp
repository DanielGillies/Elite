// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/Rocket.h"
#include "../Public/DefenderCharacter.h"
#include "../Public/AttackerCharacter.h"
#include "../Public/ElitePlayerState.h"


// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(FName("Collision Component"), TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = CollisionComp;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(FName("Particle Component"), TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = false;
	ParticleComp->bAutoDestroy = false;
	ParticleComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Movement Component"), TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 5000.0f;
	MovementComp->MaxSpeed = 5000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;


	MovementComp->OnProjectileStop.AddDynamic(this, &ARocket::OnImpact);
	CollisionComp->MoveIgnoreActors.Add(Instigator);
	

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *CollisionComp->GetName());

}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();

	//SetReplicates(true);
	bReplicateMovement = true;
	
}

// Called every frame
void ARocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARocket::LaunchProjectile(FVector ShootDirection)
{
	MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ShootDirection.ToString());
	//MovementComp->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	MovementComp->Activate();
}

void ARocket::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	if (MovementComp)
	{
		MovementComp->Velocity = NewVelocity;
	}
}

void ARocket::OnImpact(const FHitResult& HitResult)
{
	if (Role == ROLE_Authority)
	{
		Explode(HitResult);
		//DisableAndDestroy();
	}
}

void ARocket::Explode(const FHitResult& Impact)
{
	// Only Check what we hit if it is a player
	if (Cast<AFPSCharacter>(Impact.GetActor()))
	{
		if (Cast<AAttackerCharacter>(Impact.GetActor()))
		{
			AAttackerCharacter* HitCharacter = Cast<AAttackerCharacter>(Impact.GetActor());
			//FString msg = "Name " + Impact.Actor->GetName() + ", Health " + FString::FromInt(HitCharacter->Health);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
			float DamageTaken = HitCharacter->TakeDamage(1.f, FDamageEvent(), Instigator->GetController(), this);
			AElitePlayerState* PState = Cast<AElitePlayerState>(Instigator->GetController()->PlayerState);
			if (PState)
			{
				PState->Rockets += 1;
				UE_LOG(LogTemp, Warning, TEXT("%s hit a rocket on %s -- Has %d rocket hits"), *Instigator->GetName(), *HitCharacter->GetName(), PState->Rockets);
			}
			UE_LOG(LogTemp, Warning, TEXT("%f"), DamageTaken);
		}
		else
		{
			ADefenderCharacter* HitCharacter = Cast<ADefenderCharacter>(Impact.GetActor());
			//FString msg = "Name " + Impact.Actor->GetName() + ", Health " + FString::FromInt(HitCharacter->Health);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
		}
		//ADefenderCharacter *HitCharacter = Cast<ADefenderCharacter>(Impact.GetActor());
		//FString msg = "Name " + Impact.Actor->GetName() + ", Health " + FString::FromInt(HitCharacter->Health);
		/*if (ParticleComp)
		{
			ParticleComp->Deactivate();
		}*/

		// effects and damage origin shouldn't be placed inside mesh at impact point
		const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;

		/*if (WeaponConfig.ExplosionDamage > 0 && WeaponConfig.ExplosionRadius > 0 && WeaponConfig.DamageType)
		{
			UGameplayStatics::ApplyRadialDamage(this, WeaponConfig.ExplosionDamage, NudgedImpactLocation, WeaponConfig.ExplosionRadius, WeaponConfig.DamageType, TArray<AActor*>(), this, MyController.Get());
		}

		if (ExplosionTemplate)
		{
			FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), NudgedImpactLocation);
			AShooterExplosionEffect* const EffectActor = GetWorld()->SpawnActorDeferred<AShooterExplosionEffect>(ExplosionTemplate, SpawnTransform);
			if (EffectActor)
			{
				EffectActor->SurfaceHit = Impact;
				UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
			}
		}

		bExploded = true;*/
	}
	this->Destroy();

}