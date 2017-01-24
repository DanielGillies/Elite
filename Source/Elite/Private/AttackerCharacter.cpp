// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/AttackerCharacter.h"
#include "../Public/DefenderCharacter.h"
#include "../Public/MyPlayerController.h"
#include "../Public/Rocket.h"

// Sets default values
AAttackerCharacter::AAttackerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Ammo = MaxAmmo;
	Health = 3;

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
		FVector Start = (PC->PlayerCameraManager->GetCameraLocation() + (PC->GetActorForwardVector() * 50)) - FVector(0, 0, 10);
		FVector End = Start + PC->GetActorForwardVector() * 5000;

		/*ServerFireRail(Start, End);*/

		// Store the hit result
		FHitResult HitResult;

		// Set up trace params
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		FCollisionQueryParams TraceParameters;
		TraceParameters.TraceTag = TraceTag;
		TraceParameters.AddIgnoredActor(PC->GetPawn());
		TraceParameters.bTraceComplex = false;
		TraceParameters.bTraceAsyncScene = false;

		// Do line trace
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_PhysicsBody,
			FCollisionQueryParams()
		);

		UE_LOG(LogTemp, Warning, TEXT("%s"), HitResult.bBlockingHit ? *FString("Yes") : *FString("no"));
		if (HitResult.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
		}

		ServerNotifyShot(HitResult, Start, End);

	}
}

bool AAttackerCharacter::ServerNotifyShot_Validate(FHitResult HitResult, FVector Start, FVector End)
{
	return true;
}

void AAttackerCharacter::ServerNotifyShot_Implementation(FHitResult HitResult, FVector Start, FVector End)
{
	// Check if we hit an enemy
	CreateRailParticle(Start, End, HitResult);
}

//bool AAttackerCharacter::ServerFireRail_Validate(FVector Start, FVector End)
//{
//	return true;
//}


//void AAttackerCharacter::ServerFireRail_Implementation(FVector Start, FVector End)
//{
//	// Store the hit result
//	FHitResult HitResult;
//
//	// Set up trace params
//	const FName TraceTag("MyTraceTag");
//	GetWorld()->DebugDrawTraceTag = TraceTag;
//	FCollisionQueryParams TraceParameters;
//	TraceParameters.TraceTag = TraceTag;
//	TraceParameters.bTraceComplex = true;
//	TraceParameters.bTraceAsyncScene = true;
//
//	// Do line trace
//	GetWorld()->LineTraceSingleByChannel(
//		HitResult,
//		Start,
//		End,
//		ECollisionChannel::ECC_Visibility,
//		FCollisionQueryParams()
//	);
//
//	ProcessInstantHit(HitResult, Start, End);
//
//	//CreateRailParticle(Start, End, HitResult);
//
//	//UE_LOG(LogTemp, Warning, TEXT("%s"), HitResult.bBlockingHit ? *FString("Yes") : *FString("no"));
//
//}


void AAttackerCharacter::ChangeTeam()
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	PC->ChangeTeam(this);
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

void AAttackerCharacter::CreateRailParticle_Implementation(FVector Start, FVector End, FHitResult HitResult)
{
	FString netmode = "";
	if (GetNetMode() == NM_Client)
		netmode = "CLIENT";
	else if (GetNetMode() == NM_ListenServer)
		netmode = "LISTEN";
	else if (GetNetMode() == NM_DedicatedServer)
		netmode = "SERVER";

	UE_LOG(LogTemp, Warning, TEXT("%s"), *netmode);
	// Spawn rail
	UParticleSystemComponent* Rail = UGameplayStatics::SpawnEmitterAtLocation(this, RailBeam, Start);
	// If we hit something, draw rail from where we shot to where we hit
	if (HitResult.bBlockingHit)
	{
		Rail->SetBeamSourcePoint(0, Start, 0);
		Rail->SetBeamTargetPoint(0, HitResult.Location, 0);
		CheckIfHitEnemy(HitResult);
	}
	// Else draw to end of raytrace
	else
	{
		Rail->SetBeamSourcePoint(0, Start, 0);
		Rail->SetBeamTargetPoint(0, End, 0);
	}
}

bool AAttackerCharacter::CreateRailParticle_Validate(FVector Start, FVector End, FHitResult HitResult)
{
	return true;
}

void AAttackerCharacter::CheckIfHitEnemy(FHitResult HitResult)
{
	if (Cast<AFPSCharacter>(HitResult.GetActor()))
	{
		if (Cast<ADefenderCharacter>(HitResult.GetActor()))
		{
			ADefenderCharacter* HitCharacter = Cast<ADefenderCharacter>(HitResult.GetActor());
			float DamageTaken = HitCharacter->TakeDamage(1.f, FDamageEvent(), Instigator->GetController(), this);
		}
	}
}