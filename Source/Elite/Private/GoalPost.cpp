// Fill out your copyright notice in the Description page of Project Settings.

#include "Elite.h"
#include "GoalPost.h"


// Sets default values
AGoalPost::AGoalPost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoalPost::BeginPlay()
{
	Super::BeginPlay();
	
	//UE_LOG(LogTemp, Warning, TEXT("%d"), *GetComponentsByTag(AGoalPost, TEXT("Trigger")).Num)
}

// Called every frame
void AGoalPost::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGoalPost::OnBeginOverlap( AActor* Other )
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping"))
}

void AGoalPost::OnEndOverlap(AActor* Other )
{
	UE_LOG(LogTemp, Warning, TEXT("Exited"))
}