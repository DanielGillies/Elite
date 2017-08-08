// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "Weapon.h"
#include "MyPlayerController.h"
#include "ElitePlayerState.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetActorEnableCollision(false);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	//RootComponent->bHi
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

//FHitResult AWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const
//{
//	return nullptr;
//}

void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread)
{

}


// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	if (ProjectileType == EWeaponProjectile::ERail)
	{
		InstantFire();
	}
	else if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		FireProjectile();
	}
}

void AWeapon::FireProjectile()
{

	// Get Owning pawn and cast to FPSCharacter
	AFPSCharacter* OwningPawn = Cast<AFPSCharacter>(GetOwner());

	// Get Player controller
	AMyPlayerController* OwningController = Cast<AMyPlayerController>(OwningPawn->GetController());

	// Set up Player Controller to access functions
	//AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());

	// Getting Rotation from character to 3D world space of xhair
	FVector HitLocation;
	FVector Location;
	FRotator Rotation;
	if (OwningController->GetSightRayHitLocation(HitLocation, WeaponConfig.WeaponRange))
	{
		// Setting up the rotation and location of the spawn point for the rocket
		Location = OwningPawn->GetMesh()->GetSocketLocation("weapon_socket");
		Rotation = (HitLocation - (Location)).Rotation();
	}
	else
	{
		// Setting up the rotation and location of the spawn point for the rocket
		Location = OwningPawn->GetMesh()->GetSocketLocation("weapon_socket");
		Rotation = OwningPawn->GetControlRotation();
		Rotation.Pitch += 1.f;
	}

	FTransform ProjectileTransform = FTransform(Rotation, Location);
	
	ServerFireProjectile(ProjectileTransform);
}

bool AWeapon::ServerFireProjectile_Validate(FTransform ProjectileTransform)
{
	return true;
}

void AWeapon::ServerFireProjectile_Implementation(FTransform ProjectileTransform)
{
	// Set up SpawnParams for rocket
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//FTransform SpawnTM(ShootDir, Origin);
	FVector ShootDirection = ProjectileTransform.GetRotation().GetForwardVector();
	ARocket* Rocket = GetWorld()->SpawnActor<ARocket>(ProjectileBlueprint, ProjectileTransform, SpawnParams);
	if (Rocket)
	{
		//Rocket->Shooter = Shooter;
		Rocket->CollisionComp->IgnoreActorWhenMoving(this, true);
		//Rocket->CollisionComp->IgnoreActorWhenMoving(OwningPawn, true);
		Rocket->LaunchProjectile(ShootDirection);
		Rocket->SetReplicates(true);
		Rocket->bAlwaysRelevant = true;
		Rocket->bReplicateMovement = true;
	}
}

void AWeapon::InstantFire()
{
	// Get Owning pawn and cast to FPSCharacter
	AFPSCharacter* OwningPawn = Cast<AFPSCharacter>(GetOwner());

	// Get Player controller
	AMyPlayerController* OwningController = Cast<AMyPlayerController>(OwningPawn->GetController());
	FVector HitLocation;
	// Start and End vectors for line trace
	FVector Start = OwningPawn->GetMesh()->GetSocketLocation("weapon_socket");
	FVector End = Start + OwningController->GetActorForwardVector() * WeaponConfig.WeaponRange;

	// If we are aiming at object, end is where we hit the object
	if (OwningController->GetSightRayHitLocation(HitLocation, WeaponConfig.WeaponRange))
	{
		End = HitLocation;
	}

	// Store the hit result
	FHitResult HitResult(ForceInit);

	// Setup Trace params
	const FName TraceTag("WeaponTrace");
	FCollisionQueryParams TraceParameters(TraceTag, true, Instigator);
	TraceParameters.AddIgnoredActor(GetOwner());
	TraceParameters.AddIgnoredActor(this);
	TraceParameters.bTraceAsyncScene = true;

	// Line trace to check for hitting something
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TRACE_WEAPON, TraceParameters);

	ServerNotifyShot(Start, End, HitResult);
}

bool AWeapon::ServerNotifyShot_Validate(FVector Start, FVector End, FHitResult HitResult)
{
	// Check the player can fire
	return true;
}

void AWeapon::ServerNotifyShot_Implementation(FVector Start, FVector End, FHitResult HitResult)
{
	// Draw rail
	DrawRailParticle(Start, End);

	// Check if we hit enemy
	if (HitResult.bBlockingHit)
	{
		AFPSCharacter* Shooter = Cast<AFPSCharacter>(GetOwner());
		AFPSCharacter* HitCharacter = Cast<AFPSCharacter>(HitResult.GetActor());

		if (HitCharacter && Shooter)
		{
			AElitePlayerState* ShooterState = Cast<AElitePlayerState>(Shooter->PlayerState);
			AElitePlayerState* VictimState = Cast<AElitePlayerState>(HitCharacter->PlayerState);
			UE_LOG(LogTemp, Warning, TEXT("SHOOTER: %s HIT: %s"), *Shooter->PlayerState->GetName(), *HitCharacter->PlayerState->GetName());
			if (ShooterState && VictimState)
			{
				// IF our shooter and victim are on different teams, deal damage
				if (ShooterState->MyTeam != VictimState->MyTeam)
				{
					HitCharacter->TakeDamage(WeaponConfig.Damage, FDamageEvent::FDamageEvent(), Shooter->GetController(), Shooter);
				}
			}
		}
	}
}

bool AWeapon::DrawRailParticle_Validate(FVector Start, FVector End)
{
	return true;
}

void AWeapon::DrawRailParticle_Implementation(FVector Start, FVector End)
{
	UParticleSystemComponent* Rail = UGameplayStatics::SpawnEmitterAtLocation(this, RailBeam, Start);
	Rail->SetBeamSourcePoint(0, Start, 0);
	Rail->SetBeamTargetPoint(0, End, 0);
}
