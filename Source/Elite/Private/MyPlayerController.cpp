// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/MyPlayerController.h"
#include "../Public/DefenderCharacter.h"
#include "EliteGameMode.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//auto ControlledPlayer = GetControlledPlayer();

	//if (!ControlledPlayer)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("PlayerController not possessing a player"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing: %s"), *ControlledPlayer->GetName());
	//}

}

// Called every frame
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//AFPSCharacter* AMyPlayerController::GetControlledPlayer() const
//{
//	return Cast<AFPSCharacter>(GetPawn());
//}

bool AMyPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector WorldLocation;
	if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection))
	{
		return true;
	}
	return false;
}

bool AMyPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * 100000;
	// If Raycast collides with something
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}

bool AMyPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * .5f, ViewportSizeY * .5f);

	FVector LookDirection;

	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		/// Line-trace along that look direction, and see what we hit (up to max range)
		/// GetLookVectorHitLocation
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return false;
}

bool AMyPlayerController::ChangeTeam(bool FromAttacker)
{
	FVector SpawnLocation = FVector(0, 0, 0);
	APawn* OldPawn = GetPawn();
	AEliteGameMode* GameMode = (AEliteGameMode*)GetWorld()->GetAuthGameMode();

	if (FromAttacker)
	{
		TSubclassOf<ADefenderCharacter> DefenderBP = GameMode->DefenderBlueprint;
		ADefenderCharacter* NewCharacter = (ADefenderCharacter*)GetWorld()->SpawnActor(DefenderBP, &SpawnLocation, &FRotator::ZeroRotator);
		Possess(NewCharacter);
		UE_LOG(LogTemp, Warning, TEXT("Switched to defender"))
	}
	else
	{
		TSubclassOf<AAttackerCharacter> AttackerBP = GameMode->AttackerBlueprint;
		AAttackerCharacter* NewCharacter = (AAttackerCharacter*)GetWorld()->SpawnActor(AttackerBP, &SpawnLocation, &FRotator::ZeroRotator);
		Possess(NewCharacter);
		UE_LOG(LogTemp, Warning, TEXT("Switched to attacker"))
	}

	OldPawn->Destroy();
	return true;
}

