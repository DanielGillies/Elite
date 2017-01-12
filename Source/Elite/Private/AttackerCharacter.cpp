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
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AAttackerCharacter::Reload, 1.0, false);
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