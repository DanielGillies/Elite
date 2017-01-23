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


	SetupMovementComponent();
}

void AFPSCharacter::SetupMovementComponent()
{
	UCharacterMovementComponent* CharMove = GetCharacterMovement();
	CharMove->AirControl = .25;
	CharMove->BrakingDecelerationWalking = 100.0;
	CharMove->MaxAcceleration = 2000.0;
	CharMove->BrakingFriction = 1;
	CharMove->bUseSeparateBrakingFriction = true;
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

		ServerRequestSprint(bSprintActive);

		AddMovementInput(Direction, Value);
	}
}

bool AFPSCharacter::ServerRequestSprint_Validate(bool bSprintActive)
{
	return true;
}

void AFPSCharacter::ServerRequestSprint_Implementation(bool bSprintActive)
{
	if (bSprintActive)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
			// Strafing is much slower while sprinting
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
	//GetCharacterMovement()->AddImpulse(GetActorUpVector() * JumpForce, true);

	FVector WallImpact;
	if (CheckForWalls(WallImpact))
	{
		ServerRequestWalljump(WallImpact);
	}
	if (!GetCharacterMovement()->IsFalling())
	{
		//ACharacter::Jump();
		ServerRequestJump();
		//GetCharacterMovement()->AddImpulse(GetActorUpVector() * JumpForce, true);
	}
	else if (GetCharacterMovement()->IsFalling())
	{
		bSprintActive = true;
		UE_LOG(LogTemp, Warning, TEXT("%s"), bSprintActive ? *FString("True") : *FString("False"));
	}
}

bool AFPSCharacter::ServerRequestWalljump_Validate(FVector WallImpact)
{
	return true;
}

void AFPSCharacter::ServerRequestWalljump_Implementation(FVector WallImpact)
{
	GetCharacterMovement()->AddImpulse((WallImpact + (GetActorUpVector() * FVector(.7f))) * WallJumpForce, true);
}

bool AFPSCharacter::ServerRequestJump_Validate()
{
	return true;
}

void AFPSCharacter::ServerRequestJump_Implementation()
{
	GetCharacterMovement()->AddImpulse(GetActorUpVector() * JumpForce, true);
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

float AFPSCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		// If the damage kills us, set lifespan to 0 which destroys actor
		if (Health <= 0.f)
		{
			Cast<AMyPlayerController>(GetController())->Die(this);
		}
	}
	return ActualDamage;
}

void AFPSCharacter::Die(AMyPlayerController* PC)
{
	SetLifeSpan(0.001f);
	//FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this, PC->, RechargeTime, false);
}