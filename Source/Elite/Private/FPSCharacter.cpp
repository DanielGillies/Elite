// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "../Public/MyPlayerController.h"
#include "../Public/FPSCharacter.h"
#include "../Public/Rocket.h"
#include "../Public/ElitePlayerState.h"
#include "../Public/EliteGameState.h"
#include "../Public/Weapon.h"
#include "UnrealNetwork.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SetupMovementComponent();
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AFPSCharacter, CurrentWeapon);
	DOREPLIFETIME(AFPSCharacter, Health);
	DOREPLIFETIME(AFPSCharacter, DefaultWeapon);
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
	
}

bool AFPSCharacter::RemoveWeapon_Validate()
{
	return true;
}

void AFPSCharacter::RemoveWeapon_Implementation()
{
	if (CurrentWeapon) { CurrentWeapon->Destroy(); }
}

bool AFPSCharacter::SpawnWeapon_Validate(TSubclassOf<AWeapon> Weapon)
{
	return true;
}

void AFPSCharacter::SpawnWeapon_Implementation(TSubclassOf<AWeapon> Weapon)
{
	RemoveWeapon();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon, SpawnParams);
	EquipWeapon(Spawner);
	/*FName fnWeaponSocket = TEXT("weapon_socket");

	Spawner->AttachRootComponentTo(GetMesh(), fnWeaponSocket, EAttachLocation::SnapToTarget);

	CurrentWeapon = Spawner;*/
}


void AFPSCharacter::SetDefaultWeapon(TSubclassOf<AWeapon> Weapon)
{
	DefaultWeapon = Weapon;
}


//bool AFPSCharacter::EquipWeapon_Validate(AWeapon* WeaponToEquip)
//{
//	return true;
//}

void AFPSCharacter::EquipWeapon(AWeapon* WeaponToEquip)
{
	FName fnWeaponSocket = TEXT("weapon_socket");
	CurrentWeapon = WeaponToEquip;
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnWeaponSocket);

	AElitePlayerState* PS = Cast<AElitePlayerState>(PlayerState);
	PS->EquippedWeapon = WeaponToEquip;
	PS->isWeaponEquipped = true;

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

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);

	InputComponent->BindAction("Ready", IE_Pressed, this, &AFPSCharacter::OnReady);

	//InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	//InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);
}

void AFPSCharacter::OnReady()
{
	AElitePlayerState* PS = Cast<AElitePlayerState>(GetController()->PlayerState);
	PS->ToggleReady(PS);
}

void AFPSCharacter::OnFire()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *DefaultWeapon->GetName());

	// Set up Player Controller to access functions
	//AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());

	//FVector Start = (PC->PlayerCameraManager->GetCameraLocation() + (PC->GetActorForwardVector() * 50)) - FVector(0, 0, 10);
	//FVector End = Start + PC->GetActorForwardVector() * 5000;

	//UE_LOG(LogTemp, Warning, TEXT("IN CHAR: START = %s || END = %s"), *Start.ToString(), *End.ToString());

	if (CurrentWeapon) { CurrentWeapon->Fire();  }
	
	/*DefaultWeapon->FireWeapon();*/
	//DefaultWeapon->FireWeapon();
	//AElitePlayerState* PS = Cast<AElitePlayerState>(GetController()->PlayerState);

	//int Team = PS->MyTeam;

	//UE_LOG(LogTemp, Warning, TEXT("TEAM: %d"), PS->MyTeam);

	////UE_LOG(LogTemp, Warning, TEXT("TEAM: %s"), *PS->GetName());

	//if (Team == 1)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("FIRING RAIL"));
	//	FireRail();
	//}
	//else if (Team == 2)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("FIRING ROCKET"));
	//	FireRocket();
	//}
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
	GetCharacterMovement()->AddImpulse((WallImpact + (GetActorUpVector() * FVector(WallJumpUpForce))) * WallJumpForce, true);
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
	//UE_LOG(LogTemp, Warning, TEXT("INSTIGATOR: %s"), *EventInstigator->GetName());
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		/*UE_LOG(LogTemp, Warning, TEXT("Remaining health: %f"), Health);*/
		// If the damage kills us, set lifespan to 0 which destroys actor
		if (Health <= 0.f)
		{
			// Add score to killer's team;
			const AElitePlayerState* KillerPS = Cast<AElitePlayerState>(EventInstigator->PlayerState);
			const int KillerTeam = KillerPS->MyTeam;

			UE_LOG(LogTemp, Warning, TEXT("ADDING %d to team %d"), 1, KillerTeam);

			AEliteGameState* GS = Cast<AEliteGameState>(GetWorld()->GetGameState());
			GS->AddScoreToTeam(1, KillerTeam);

			// Kill player
			Cast<AMyPlayerController>(GetController())->Die();
		}
	}
	return ActualDamage;
}

/*Rocket Stuff*/
void AFPSCharacter::FireRocket()
{
	//if (CanFire())
	//{
	//	// Set up Player Controller to access functions
	//	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());

	//	// Getting Rotation from character to 3D world space of xhair
	//	FVector HitLocation;
	//	FVector Location;
	//	FRotator Rotation;
	//	if (PC->GetSightRayHitLocation(HitLocation))
	//	{
	//		// Setting up the rotation and location of the spawn point for the rocket
	//		Location = GetActorLocation() + (GetActorForwardVector() * 100);
	//		Rotation = (HitLocation - (Location)).Rotation();
	//	}
	//	else
	//	{
	//		// Setting up the rotation and location of the spawn point for the rocket
	//		Location = GetActorLocation() + (GetActorForwardVector() * 100);
	//		Rotation = GetControlRotation();
	//		Rotation.Pitch += 1.f;
	//	}

	//	FTransform ProjectileTransform = FTransform(Rotation, Location);

	//	ServerFireProjectile(ProjectileTransform, GetController());

	//}
}

bool AFPSCharacter::ServerFireProjectile_Validate(FTransform ProjectileTransform, AController* Shooter)
{
	return true;
}

void AFPSCharacter::ServerFireProjectile_Implementation(FTransform ProjectileTransform, AController* Shooter)
{

	FString netmode = "";
	if (GetNetMode() == NM_Client)
		netmode = "CLIENT";
	else if (GetNetMode() == NM_ListenServer)
		netmode = "LISTEN";
	else if (GetNetMode() == NM_DedicatedServer)
		netmode = "SERVER";

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *netmode);

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

/*Rail gun stuff*/

void AFPSCharacter::FireRail()
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

		ServerNotifyShot(HitResult, Start, End, GetController()->PlayerState);

	}
}

bool AFPSCharacter::ServerNotifyShot_Validate(FHitResult HitResult, FVector Start, FVector End, APlayerState* Shooter)
{
	return true;
}

void AFPSCharacter::ServerNotifyShot_Implementation(FHitResult HitResult, FVector Start, FVector End, APlayerState* Shooter)
{
	// Check if we hit an enemy
	CreateRailParticle(Start, End, HitResult, Shooter);
}


bool AFPSCharacter::CreateRailParticle_Validate(FVector Start, FVector End, FHitResult HitResult, APlayerState* Shooter)
{
	return true;
}

void AFPSCharacter::CreateRailParticle_Implementation(FVector Start, FVector End, FHitResult HitResult, APlayerState* Shooter)
{
	FString netmode = "";
	if (GetNetMode() == NM_Client)
		netmode = "CLIENT";
	else if (GetNetMode() == NM_ListenServer)
		netmode = "LISTEN";
	else if (GetNetMode() == NM_DedicatedServer)
		netmode = "SERVER";

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *netmode);
	// Spawn rail
	UParticleSystemComponent* Rail = UGameplayStatics::SpawnEmitterAtLocation(this, RailBeam, Start);
	// If we hit something, draw rail from where we shot to where we hit
	if (HitResult.bBlockingHit)
	{
		Rail->SetBeamSourcePoint(0, Start, 0);
		Rail->SetBeamTargetPoint(0, HitResult.Location, 0);
		CheckIfHitEnemy(HitResult, Shooter);
	}
	// Else draw to end of raytrace
	else
	{
		Rail->SetBeamSourcePoint(0, Start, 0);
		Rail->SetBeamTargetPoint(0, End, 0);
	}
}

void AFPSCharacter::CheckIfHitEnemy(FHitResult HitResult, APlayerState* Shooter)
{

	AElitePlayerState* ShooterPS = Cast<AElitePlayerState>(Shooter);
	if (!ShooterPS)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ShooterPS"));
		return;
	}

	AFPSCharacter* HitCharacter = Cast<AFPSCharacter>(HitResult.GetActor());
	if (HitCharacter)
	{
		//float DamageTaken = HitCharacter->TakeDamage(1.f, FDamageEvent(), Instigator->GetController(), this);
		AMyPlayerController* HitController = Cast<AMyPlayerController>(HitCharacter->GetController());
		if (HitController)
		{
			AElitePlayerState* HitPlayerState = Cast<AElitePlayerState>(HitController->PlayerState);
			if (HitPlayerState)
			{
				AEliteGameState* GS = Cast<AEliteGameState>(GetWorld()->GetGameState());
				if (GS)
				{
					if (HitPlayerState->MyTeam != ShooterPS->MyTeam)
					{
						UE_LOG(LogTemp, Warning, TEXT("HIT TEAM = %d || Attacking Team = %d"), HitPlayerState->MyTeam, GS->AttackingTeam);
						//ADefenderCharacter* HitCharacter = Cast<ADefenderCharacter>(HitResult.GetActor());
						//float DamageTaken = HitCharacter->TakeDamage(1.f, FDamageEvent(), Instigator->GetController(), this);
					}
				}
				
			}
		}
	}
}


bool AFPSCharacter::CanFire()
{
	/*if (RailAmmo > 0)
	{
	RailAmmo -= 1;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AFPSCharacter::Reload, RechargeTime, false);
	return true;
	}
	else
	{
	return false;
	}*/
	return true;
}

void AFPSCharacter::Reload()
{
	RailAmmo = MaxAmmo;
}
