// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/MyPlayerController.h"
#include "../Public/DefenderCharacter.h"
#include "EliteGameMode.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<AFPSCharacter>(GetPawn()))
	{
		if (Cast<AAttackerCharacter>(GetPawn()))
		{
			Attacking = true;
		}
		else
		{
			Attacking = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not possessing a player"));
	}
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

AFPSCharacter* AMyPlayerController::GetControlledPlayer() const
{
	return Cast<AFPSCharacter>(GetPawn());
}

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
	// TODO: Maybe change this to ECC_Camera?
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

void AMyPlayerController::Die(AFPSCharacter* Caller)
{
	Caller->SetLifeSpan(0.001f);
	FTimerHandle UnusedHandle;
	AMyPlayerController* PC = Cast<AMyPlayerController>(Caller->GetController());
	GetWorldTimerManager().SetTimer(UnusedHandle, PC, &AMyPlayerController::Respawn, 4.0f);
}

void AMyPlayerController::Respawn()
{
	UE_LOG(LogTemp, Warning, TEXT("RESPAWN"));
	//UE_LOG(LogTemp, Warning, TEXT("RESPAWN %s"), *Cast<AFPSCharacter>(GetPawn())->GetName());
	ServerRespawn();
}

bool AMyPlayerController::ServerRespawn_Validate()
{
	return true;
}

void AMyPlayerController::ServerRespawn_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("IN IT"));
	FVector SpawnLocation = FVector(0, 0, 0);
	AEliteGameMode* GameMode = (AEliteGameMode*)GetWorld()->GetAuthGameMode();
	UE_LOG(LogTemp, Warning, TEXT("PRE CAST"));
	/*AMyPlayerController* PC = Cast<AMyPlayerController>(Caller->GetController());*/
	UE_LOG(LogTemp, Warning, TEXT("POST"));
	//APawn* OldPawn = PC->GetPawn();

	if (!Attacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("Respawning Defender"));
		TSubclassOf<ADefenderCharacter> DefenderBP = GameMode->DefenderBlueprint;
		ADefenderCharacter* NewCharacter = (ADefenderCharacter*)GetWorld()->SpawnActor(DefenderBP, &SpawnLocation, &FRotator::ZeroRotator);
		UnPossess();
		Possess(NewCharacter);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Respawning Attacker"));
		TSubclassOf<AAttackerCharacter> AttackerBP = GameMode->AttackerBlueprint;
		AAttackerCharacter* NewCharacter = (AAttackerCharacter*)GetWorld()->SpawnActor(AttackerBP, &SpawnLocation, &FRotator::ZeroRotator);
		UnPossess();
		Possess(NewCharacter);
	}
}

void AMyPlayerController::ChangeTeam_Implementation(AFPSCharacter* Caller)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), Attacking ? *FString("Attacking") : *FString("Defending"));
	FVector SpawnLocation = FVector(0, 0, 0);
	AEliteGameMode* GameMode = (AEliteGameMode*)GetWorld()->GetAuthGameMode();
	AMyPlayerController* PC = Cast<AMyPlayerController>(Caller->GetController());
	APawn* OldPawn = PC->GetPawn();

	if (!Attacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEFENDER SWITCHING"));
		TSubclassOf<AAttackerCharacter> AttackerBP = GameMode->AttackerBlueprint;
		AAttackerCharacter* NewCharacter = (AAttackerCharacter*)GetWorld()->SpawnActor(AttackerBP, &SpawnLocation, &FRotator::ZeroRotator);
		PC->UnPossess();
		PC->Possess(NewCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATTACKER SWITCHING"));
		TSubclassOf<ADefenderCharacter> DefenderBP = GameMode->DefenderBlueprint;
		ADefenderCharacter* NewCharacter = (ADefenderCharacter*)GetWorld()->SpawnActor(DefenderBP, &SpawnLocation, &FRotator::ZeroRotator);
		PC->UnPossess();
		PC->Possess(NewCharacter);
	}
	Attacking = !Attacking;
	OldPawn->Destroy();
}

bool AMyPlayerController::ChangeTeam_Validate(AFPSCharacter* Caller)
{
	return true;
}


