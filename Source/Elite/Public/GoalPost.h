// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GoalPost.generated.h"

UCLASS()
class ELITE_API AGoalPost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalPost();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OnBeginOverlap(AActor* Other);

	UFUNCTION()
	void OnEndOverlap(AActor* Other);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = timer)
	float CaptureTime = 1.5f;
	
	UPROPERTY(BlueprintReadWrite)
	float ElapsedTime = 0.0f;
};
