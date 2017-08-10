// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/MyPlayerController.h"
#include "EliteGameMode.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledPlayer = GetControlledPlayer();

	if (!ControlledPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("%s not possessing a player"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s possessing: %s"), *GetName(), *ControlledPlayer->GetName());
	}

}

// Called every frame
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPlayerController::SetTeam(int Team)
{
	MyTeam = Team;
	UE_LOG(LogTemp, Warning, TEXT("%s JOINED TEAM %d"), *GetName(), MyTeam)
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

bool AMyPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation, float WeaponRange) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * WeaponRange;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetPawn());

	// If Raycast collides with something
	// TODO: Maybe change this to ECC_Camera?
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_WorldStatic, CollisionParams))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}

bool AMyPlayerController::GetSightRayHitLocation(FVector& HitLocation, float WeaponRange) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * .5f, ViewportSizeY * .5f);

	FVector LookDirection;

	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		/// Line-trace along that look direction, and see what we hit (up to max range)
		/// GetLookVectorHitLocation
		return GetLookVectorHitLocation(LookDirection, HitLocation, WeaponRange);
	}

	return false;
}

void AMyPlayerController::Die()
{
	GetPawn()->SetLifeSpan(0.001f);
	//AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AMyPlayerController::Respawn, 4.0f);
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
	FVector SpawnLocation = FVector(0, 0, 0);
	APawn* OldPawn = GetPawn();

	/*TSubclassOf<AFPSCharacter> FPSCharacterBP = CharacterBlueprint;*/
	AEliteGameMode* GameMode = (AEliteGameMode*)GetWorld()->GetAuthGameMode();
	AFPSCharacter* NewCharacter = (AFPSCharacter*)GetWorld()->SpawnActor(GameMode->CharacterBlueprint, &SpawnLocation, &FRotator::ZeroRotator);
	UnPossess();
	Possess(NewCharacter);
	//AFPSCharacter* NewCharacter = new AFPSCharacter
	//UE_LOG(LogTemp, Warning, TEXT("IN IT"));
	//FVector SpawnLocation = FVector(0, 0, 0);
	//AEliteGameMode* GameMode = (AEliteGameMode*)GetWorld()->GetAuthGameMode();
	//UE_LOG(LogTemp, Warning, TEXT("PRE CAST"));
	///*AMyPlayerController* PC = Cast<AMyPlayerController>(Caller->GetController());*/
	//UE_LOG(LogTemp, Warning, TEXT("POST"));
	////APawn* OldPawn = PC->GetPawn();

	//if (!Attacking)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Respawning Defender"));
	//	TSubclassOf<ADefenderCharacter> DefenderBP = GameMode->DefenderBlueprint;
	//	ADefenderCharacter* NewCharacter = (ADefenderCharacter*)GetWorld()->SpawnActor(DefenderBP, &SpawnLocation, &FRotator::ZeroRotator);
	//	UnPossess();
	//	Possess(NewCharacter);
	//	
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Respawning Attacker"));
	//	TSubclassOf<AAttackerCharacter> AttackerBP = GameMode->AttackerBlueprint;
	//	AAttackerCharacter* NewCharacter = (AAttackerCharacter*)GetWorld()->SpawnActor(AttackerBP, &SpawnLocation, &FRotator::ZeroRotator);
	//	UnPossess();
	//	Possess(NewCharacter);
	//}
}