// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/Rocket.h"
#include "../Public/ElitePlayerState.h"
#include "../Public/MyPlayerController.h"


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

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(FName("Radial Force Component"), TEXT("RadialForceComp"));
	RadialForceComp->ImpulseStrength = 65000.f;
	RadialForceComp->DestructibleDamage = 1.0f;
	//RadialForceComp->ForceStrength = 500000.f;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->Radius = 200.f;
	RadialForceComp->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialForceComp->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;

	MovementComp->OnProjectileStop.AddDynamic(this, &ARocket::OnImpact);
	CollisionComp->MoveIgnoreActors.Add(Instigator);

}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();

	//SetReplicates(true);
	bReplicateMovement = true;

	//FString netmode = "";
	//if (GetNetMode() == NM_Client)
	//	netmode = "CLIENT";
	//else if (GetNetMode() == NM_ListenServer)
	//	netmode = "LISTEN";
	//else if (GetNetMode() == NM_DedicatedServer)
	//	netmode = "SERVER";

	//UE_LOG(LogTemp, Warning, TEXT("%s: ROCKET INSTIGATOR %s"), *netmode, *Instigator->GetName());
	
}

// Called every frame
void ARocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARocket::LaunchProjectile(FVector ShootDirection)
{
	/*UE_LOG(LogTemp, Warning, TEXT("Shooter = %s"), *Shooter->GetName());*/
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
		//UE_LOG(LogTemp, Warning, TEXT("ROCKET INSTIGATOR %s"), *Instigator->GetName());
		Explode(HitResult);
		//DisableAndDestroy();
	}
}

void ARocket::Explode(const FHitResult& Impact)
{
	//Instigator->PlayerState;
	//AMyPlayerController* CurrShooter = Cast<AMyPlayerController>(Shooter);
	AElitePlayerState* ShooterPS = Cast<AElitePlayerState>(Instigator->PlayerState);
	
	AFPSCharacter* Victim = Cast<AFPSCharacter>(Impact.GetActor());
	if (Victim)
	{
		AMyPlayerController* VictimController = Cast<AMyPlayerController>(Victim->GetController());
		if (VictimController)
		{
			AElitePlayerState* VictimPS = Cast<AElitePlayerState>(VictimController->PlayerState);
			if (VictimPS)
			{
				
				UE_LOG(LogTemp, Warning, TEXT("SHOOTER TEAM = %d || VICTIM TEAM = %d"), ShooterPS->MyTeam, VictimPS->MyTeam);
				if (VictimPS->MyTeam != ShooterPS->MyTeam)
				{
					//float DamageTaken = Victim->TakeDamage(1.f, FDamageEvent(), Instigator->GetController(), this);
					if (ShooterPS)
					{
						ShooterPS->Rockets += 1;
						//UE_LOG(LogTemp, Warning, TEXT("%s hit a rocket on %s -- Has %d rocket hits"), *Shooter->GetName(), *Victim->GetName(), ShooterPS->Rockets);
					}
				}
			}
		}
	}
	//// Only Check what we hit if it is a player
	//if (Cast<AFPSCharacter>(Impact.GetActor()))
	//{
	//	if (Cast<AAttackerCharacter>(Impact.GetActor()))
	//	{
	//		AAttackerCharacter* HitCharacter = Cast<AAttackerCharacter>(Impact.GetActor());
	//		//FString msg = "Name " + Impact.Actor->GetName() + ", Health " + FString::FromInt(HitCharacter->Health);
	//		//UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
	//		float DamageTaken = HitCharacter->TakeDamage(1.f, FDamageEvent(), Instigator->GetController(), this);
	//		AElitePlayerState* PState = Cast<AElitePlayerState>(Instigator->GetController()->PlayerState);
	//		if (PState)
	//		{
	//			PState->Rockets += 1;
	//			UE_LOG(LogTemp, Warning, TEXT("%s hit a rocket on %s -- Has %d rocket hits"), *Instigator->GetName(), *HitCharacter->GetName(), PState->Rockets);
	//		}
	//		UE_LOG(LogTemp, Warning, TEXT("%f"), DamageTaken);
	//	}
	//	else
	//	{
	//		ADefenderCharacter* HitCharacter = Cast<ADefenderCharacter>(Impact.GetActor());
	//		//FString msg = "Name " + Impact.Actor->GetName() + ", Health " + FString::FromInt(HitCharacter->Health);
	//		//UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
	//	}
	//	//ADefenderCharacter *HitCharacter = Cast<ADefenderCharacter>(Impact.GetActor());
	//	//FString msg = "Name " + Impact.Actor->GetName() + ", Health " + FString::FromInt(HitCharacter->Health);
	//	/*if (ParticleComp)
	//	{
	//		ParticleComp->Deactivate();
	//	}*/

	//	// effects and damage origin shouldn't be placed inside mesh at impact point
	//	const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;

	//	/*if (WeaponConfig.ExplosionDamage > 0 && WeaponConfig.ExplosionRadius > 0 && WeaponConfig.DamageType)
	//	{
	//		UGameplayStatics::ApplyRadialDamage(this, WeaponConfig.ExplosionDamage, NudgedImpactLocation, WeaponConfig.ExplosionRadius, WeaponConfig.DamageType, TArray<AActor*>(), this, MyController.Get());
	//	}

	//	if (ExplosionTemplate)
	//	{
	//		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), NudgedImpactLocation);
	//		AShooterExplosionEffect* const EffectActor = GetWorld()->SpawnActorDeferred<AShooterExplosionEffect>(ExplosionTemplate, SpawnTransform);
	//		if (EffectActor)
	//		{
	//			EffectActor->SurfaceHit = Impact;
	//			UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
	//		}
	//	}

	//	bExploded = true;*/
	//}
	//this->Destroy();
	//UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 80.f, 5.f, Impact.ImpactPoint, 80.f, 5.f, 10.f, )
	const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 200.f, 20.f, NudgedImpactLocation, 50.f, 100.f, 10.f, UDamageType::StaticClass(), TArray<AActor*>(), GetOwner(), Instigator->GetController());
	//UGameplayStatics::ApplyRadialDamage(GetWorld(), 80.f, Impact.ImpactPoint, 75.f, UDamageType::StaticClass(), TArray<AActor*>(), GetOwner(), Instigator->GetController());
	RadialForceComp->FireImpulse();

	Destroy();

}