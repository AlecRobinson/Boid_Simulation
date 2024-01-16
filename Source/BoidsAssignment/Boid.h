// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDSASSIGNMENT_API ABoid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoid();

	FVector currentForce = FVector::ZeroVector;
	class ABoidManager* Manager;
	class ABoid* BoidActorClass;

	//Wander variables
	int circleDistance = 2;
	int wanderAngle = 50;
	int angleChange = 50;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Functions for boid movement
	FVector Seek(FVector position);
	FVector Flee(FVector position);
	FVector Wander(float radius, float distance, float jitter);
	FVector WanderDestination;
	FVector Pursue(ABoid* prey);
	FVector Evade(class ABoidLeader* shark);
	FVector ObjectAvoidance();

	//Properties for visuals on boid
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* BoidRootComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USceneComponent* MeshParent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* Mesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoids(float DeltaTime);

	//Nearby boids array function
	TArray<class ABoid*> NearbyBoidPositions(ABoid* thisBoid, float range);

	//Flocking functions
	FVector Cohesion();
	FVector Separation();
	FVector Alignment();
	FVector MoveTowardOrigin();
	FVector FindOtherBoids();

	//Setting and changing mesh with movement
	void RotateToFace();
	void SetManagerAndMesh(class ABoidManager* manager);
};
