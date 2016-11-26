// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/AttackerCharacter.h"
#include "../Public/MyPlayerController.h"
#include "../Public/Rocket.h"

// Sets default values
AAttackerCharacter::AAttackerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAttackerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->InitialPushForceFactor);
}

// Called every frame
void AAttackerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	//FVector LookDirection = GetActorLocation();

	//PC->GetLookDirection(FVector2D(.5f, .5f), LookDirection);
}

void AAttackerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AAttackerCharacter::OnFire);

	InputComponent->BindAction("ChangeTeam", IE_Pressed, this, &AAttackerCharacter::ChangeTeam);
}


void AAttackerCharacter::OnFire()
{
	// Set up SpawnParams for rocket
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SpawnParams.Instigator = Instigator;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Set up Player Controller to access functions
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());

	// Getting Rotation from character to 3D world space of xhair
	FVector HitLocation;
	FVector Location;
	FRotator Rotation;

	/*const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams TraceParameters;
	TraceParameters.TraceTag = TraceTag;

	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

	FVector End = CameraLocation + PC->PlayerCameraManager->GetActorForwardVector() * 10000;
	FVector Start = CameraLocation + PC->PlayerCameraManager->GetActorForwardVector() * 500;
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		TraceParameters);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());*/

	/*DrawDebugLine(
		GetWorld(),
		GetReachLineStart(),
		GetReachLineEnd(),
		FColor(255, 0, 0),
		false, 100, 0,
		12.333
	);*/

	/*FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);*/

	//if (PC->GetSightRayHitLocation(HitLocation))
	//{
	//	// Setting up the rotation and location of the spawn point for the rocket
	//	Location = GetActorLocation() + (GetActorForwardVector() * 100);
	//	Rotation = (HitLocation - (Location)).Rotation();
	//}
	//else
	//{
	//	// Setting up the rotation and location of the spawn point for the rocket
	//	Location = GetActorLocation() + (GetActorForwardVector() * 100);
	//	Rotation = GetControlRotation();
	//	Rotation.Pitch += 1.f;
	//}

	//FTransform ProjectileTransform = FTransform(Rotation, Location, FVector(0));

	//// Spawn the rocket using the rocket blueprint
	//ARocket* Rocket = GetWorld()->SpawnActor<ARocket>(RocketBlueprint, ProjectileTransform, SpawnParams);
	//Rocket->LaunchProjectile(LaunchSpeed);
}

void AAttackerCharacter::ChangeTeam()
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	PC->ChangeTeam(true);
}