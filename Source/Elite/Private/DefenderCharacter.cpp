// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/DefenderCharacter.h"
#include "../Public/MyPlayerController.h"
#include "../Public/Rocket.h"

// Sets default values
ADefenderCharacter::ADefenderCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetupMovementComponent();

	Ammo = MaxAmmo;
	Health = 1;
}

// Called when the game starts or when spawned
void ADefenderCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->InitialPushForceFactor);
}

// Called every frame
void ADefenderCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	//FVector LookDirection = GetActorLocation();

	//PC->GetLookDirection(FVector2D(.5f, .5f), LookDirection);
}

void ADefenderCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ADefenderCharacter::OnFire);
	InputComponent->BindAction("ChangeTeam", IE_Pressed, this, &ADefenderCharacter::ChangeTeam);
}


void ADefenderCharacter::OnFire()
{
	if (CanFire())
	{
		// Set up Player Controller to access functions
		AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());

		// Getting Rotation from character to 3D world space of xhair
		FVector HitLocation;
		FVector Location;
		FRotator Rotation;
		if (PC->GetSightRayHitLocation(HitLocation))
		{
			// Setting up the rotation and location of the spawn point for the rocket
			Location = GetActorLocation() + (GetActorForwardVector() * 100);
			Rotation = (HitLocation - (Location)).Rotation();
		}
		else
		{
			// Setting up the rotation and location of the spawn point for the rocket
			Location = GetActorLocation() + (GetActorForwardVector() * 100);
			Rotation = GetControlRotation();
			Rotation.Pitch += 1.f;
		}

		FTransform ProjectileTransform = FTransform(Rotation, Location);

		ServerFireProjectile(ProjectileTransform);

			//// Spawn the rocket using the rocket blueprint
			//ARocket* Rocket = GetWorld()->SpawnActor<ARocket>(RocketBlueprint, ProjectileTransform, SpawnParams);
			//FVector ShootDirection = ProjectileTransform.GetRotation().GetForwardVector();
			//Print("CLIENT ROTATION:   " + ShootDirection.ToString());
			//Rocket->LaunchProjectile(ShootDirection);
			//Print("CLIENT SPAWN:   " + Rocket->GetVelocity().ToString());
			//Rocket->SetReplicates(true);
			//Rocket->bAlwaysRelevant = true;
			//Rocket->bReplicateMovement = true;

			//Ammo -= 1;

			//FTimerHandle UnusedHandle;
			//GetWorldTimerManager().SetTimer(UnusedHandle, this, &ADefenderCharacter::Reload, RechargeTime, false);

	}

}

bool ADefenderCharacter::ServerFireProjectile_Validate(FTransform ProjectileTransform)
{
	return true;
}

void ADefenderCharacter::ServerFireProjectile_Implementation(FTransform ProjectileTransform)
{

	// Set up SpawnParams for rocket
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//FTransform SpawnTM(ShootDir, Origin);
	FVector ShootDirection = ProjectileTransform.GetRotation().GetForwardVector();
	ARocket* Rocket = GetWorld()->SpawnActor<ARocket>(RocketBlueprint, ProjectileTransform, SpawnParams);
	if (Rocket)
	{
		Rocket->LaunchProjectile(ShootDirection);
		Rocket->SetReplicates(true);
		Rocket->bAlwaysRelevant = true;
		Rocket->bReplicateMovement = true;


	}
}

void ADefenderCharacter::ChangeTeam()
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	PC->ChangeTeam(this);
}

void ADefenderCharacter::SetupMovementComponent()
{
	Super::SetupMovementComponent();
}

bool ADefenderCharacter::CanFire()
{
	return Ammo > 0;
}

void ADefenderCharacter::Reload()
{
	Ammo++;
}

int ADefenderCharacter::GetCurrentAmmo()
{
	return Ammo;
}

void ADefenderCharacter::Print(FString Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}