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
	TArray<AActor*> IgnoredActors;
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
					UGameplayStatics::ApplyDamage(Victim, 70.f, Instigator->GetController(), Instigator, UDamageType::StaticClass());
					// If we direct hit, ignore the actor that we hit for the radial damage call
					IgnoredActors.Add(Victim);
					if (ShooterPS)
					{
						ShooterPS->Rockets += 1;
						//UE_LOG(LogTemp, Warning, TEXT("%s hit a rocket on %s -- Has %d rocket hits"), *Shooter->GetName(), *Victim->GetName(), ShooterPS->Rockets);
					}
				}
			}
		}
	}

	//const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;
	
	IgnoredActors.Add(GetOwner());
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 50.f, 5.f, Impact.ImpactPoint, 50.f, 145.f, 10.f, UDamageType::StaticClass(), IgnoredActors, GetOwner(), Instigator->GetController());
	//UGameplayStatics::ApplyRadialDamage(GetWorld(), 50.f, Impact.ImpactPoint, 160.f, UDamageType::StaticClass(), IgnoredActors, GetOwner(), Instigator->GetController());
	RadialForceComp->FireImpulse();

	Destroy();

}