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

	Ammo = MaxAmmo;

	SetupMovementComponent();
}

// Called when the game starts or when spawned
void AAttackerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAttackerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAttackerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AAttackerCharacter::OnFire);

	InputComponent->BindAction("ChangeTeam", IE_Pressed, this, &AAttackerCharacter::ChangeTeam);
}

// Currently being handled by blueprint
void AAttackerCharacter::OnFire()
{
	if (CanFire())
	{
		// Set up Player Controller to access functions
		AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());

		// Start and end vectors for line trace
		FVector Start = PC->PlayerCameraManager->GetCameraLocation() - FVector(0, 0, 10);;
		FVector End = Start + PC->GetActorForwardVector() * 5000;

		// Store the hit result
		FHitResult HitResult;

		// Set up trace params
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		FCollisionQueryParams TraceParameters;
		TraceParameters.TraceTag = TraceTag;
		/*TraceParameters.bTraceComplex = true;
		TraceParameters.bTraceAsyncScene = true;*/

		// Do line trace
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			FCollisionQueryParams()
		);

		// Spawn rail
		UParticleSystemComponent* Rail = UGameplayStatics::SpawnEmitterAtLocation(this, RailBeam, Start);

		// If we hit something, draw rail from where we shot to where we hit
		if (HitResult.bBlockingHit)
		{
			Rail->SetBeamSourcePoint(0, Start, 0);
			Rail->SetBeamTargetPoint(0, HitResult.Location, 0);
		}
		// Else draw to end of raytrace
		else
		{
			Rail->SetBeamSourcePoint(0, Start, 0);
			Rail->SetBeamTargetPoint(0, End, 0);
		}
	}
}

void AAttackerCharacter::ChangeTeam()
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	PC->ChangeTeam(true);
}

void AAttackerCharacter::SetupMovementComponent()
{
	Super::SetupMovementComponent();
}

bool AAttackerCharacter::CanFire()
{
	if (Ammo > 0)
	{
		Ammo -= 1;

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AAttackerCharacter::Reload, RechargeTime, false);
		return true;
	}
	else
	{
		return false;
	}
}

void AAttackerCharacter::Reload()
{
	Ammo = MaxAmmo;
}