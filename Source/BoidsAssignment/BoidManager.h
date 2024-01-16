// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

UCLASS()
class BOIDSASSIGNMENT_API ABoidManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoidManager();

	//Boid Info Non Editable In Game
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SetUpSettings")
	UStaticMesh* BoidMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SetUpSettings")
	int spawnCount = 60;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SetUpSettings")
	float spawnRadius = 1000.0f;

	// Boid Info Editable In Game
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float MaxFleeDistance = 3000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float raduisOfDetectedBoids = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float maxVelocity = 0.5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float speed = 5;

	//Flocking Info
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flocking", meta = (UIMin = "0.0", UIMax = "1.0"))
	float cohesionPercent = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flocking")
	float separationLength = 300.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flocking", meta = (UIMin = "0.0", UIMax = "1.0"))
	float seperationPercent = 0.40f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flocking", meta = (UIMin = "0.0", UIMax = "1.0"))
	float alignmentPercent = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flocking", meta = (UIMin = "0.0", UIMax = "1.0"))
	float fleePercent = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debugging")
	bool debugLines = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debugging")
	bool containmentBounds = true;

	//Shark Info
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shark")
	float sharkSpeed = 30;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shark", meta = (UIMin = "0.0", UIMax = "1.0"))
	float seekPercent = 0.7f;

	//Other variables
	USceneComponent* transform;
	TArray<class ABoid*> myBoids;
	class ABoidLeader* leaderboid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
