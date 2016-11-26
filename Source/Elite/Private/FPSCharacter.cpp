// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/MyPlayerController.h"
#include "../Public/FPSCharacter.h"
#include "../Public/Rocket.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->InitialPushForceFactor);
}

// Called every frame
void AFPSCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	//FVector LookDirection = GetActorLocation();

	//PC->GetLookDirection(FVector2D(.5f, .5f), LookDirection);
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::JumpReleased);

	//InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	//InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);
}


// Move Forward
void AFPSCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// Find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// Move in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

		if (bSprintActive)
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}

		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	// Find out which direction is right
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		
		if (bSprintActive)
		{
			// Stafing is much slower while sprinting
			AddMovementInput(Direction, Value * StrafeSpeedMult);
		}
		else
		{
			// Add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

void AFPSCharacter::Jump()
{
	FVector WallImpact;
	if (CheckForWalls(WallImpact))
	{
		GetCharacterMovement()->AddImpulse((WallImpact + (GetActorUpVector() * FVector(.7f))) * WallJumpForce, true);
	}
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->AddImpulse(GetActorUpVector() * JumpForce, true);
	}
	else if (GetCharacterMovement()->IsFalling())
	{
		bSprintActive = true;
	}
}

void AFPSCharacter::JumpReleased()
{
	bSprintActive = false;
}

void AFPSCharacter::OnStartJump()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bPressedJump = false;
		bPressedJump = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), GetCharacterMovement()->IsFalling() ? TEXT("In air") : TEXT("On Ground"));

	if (GetCharacterMovement()->IsFalling())
	{
		bSprintActive = true;
	}
}

void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
	bSprintActive = false;
}

bool AFPSCharacter::CheckForWalls(FVector& OutImpactVector)
{
	TArray<FOverlapResult> Overlaps;
	AActor* Origin = 0;
	FVector CurrentLocation = GetTransform().GetLocation();

	FHitResult LeftHit;
	FHitResult RightHit;

	if (GetCharacterMovement()->IsFalling())
	{

		if (CheckLeftForWalls(LeftHit))
		{
			UE_LOG(LogTemp, Warning, TEXT("LEFT VECTOR: %s"), *LeftHit.ImpactNormal.ToString());
			OutImpactVector = LeftHit.ImpactNormal;
			return true;
		}
		else if (CheckRightForWalls(RightHit))
		{
			UE_LOG(LogTemp, Warning, TEXT("RIGHT VECTOR: %s"), *RightHit.ImpactNormal.ToString());
			OutImpactVector = RightHit.ImpactNormal;
			return true;
		}
	}
	return false;
}

bool AFPSCharacter::CheckLeftForWalls(FHitResult& OutHitResult)
{
	FVector CurrentLocation = GetTransform().GetLocation();

	return GetWorld()->LineTraceSingleByObjectType(OutHitResult, CurrentLocation, CurrentLocation + (GetActorRightVector() * -1) * WallCheckRadius, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic));
	
}

bool AFPSCharacter::CheckRightForWalls(FHitResult& OutHitResult)
{
	FVector CurrentLocation = GetTransform().GetLocation();

	return GetWorld()->LineTraceSingleByObjectType(OutHitResult, CurrentLocation, CurrentLocation + GetActorRightVector() * WallCheckRadius, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic));

}