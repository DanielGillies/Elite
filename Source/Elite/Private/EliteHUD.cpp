// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "EliteHUD.h"
#include "EliteGameMode.h"


AEliteHUD::AEliteHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = HUDFontOb.Object;

}


void AEliteHUD::DrawHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("DrawHUD called"));
	// Get Screen Directions
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	// Call parent DrawHUD
	Super::DrawHUD();

	//ABatteryGameCharacter* MyCharacter = Cast<ABatteryGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FString PowerLevelString = FString::Printf(TEXT("%10.1"), FMath::Abs(10));//MyCharacter->PowerLevel));

	DrawText(PowerLevelString, FColor::White, 50, 50, HUDFont);

	//ABatteryGameGameMode* MyGameMode = Cast<ABatteryGameGameMode>(UGameplayStatics::GetGameMode(this));

	//     if (MyGameMode->GetCurrentState() == EBatteryGamePlayState::EGameOver)
	//     {
	//         FVector2D GameOverSize;
	//         GetTextSize(TEXT("GAME OVER"), GameOverSize.X, GameOverSize.Y, HUDFont);
	// 
	//         DrawText(TEXT("GAME OVER"), FColor::White, (ScreenDimensions.X - GameOverSize.X) / 2.0f, (ScreenDimensions.Y - GameOverSize.Y) / 2.0f, HUDFont);
	//     }
}

