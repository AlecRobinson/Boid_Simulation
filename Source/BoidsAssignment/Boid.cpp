// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"
#include "BoidManager.h"
#include "BoidLeader.h"
#include "Components/SphereComponent.h"
#include <math.h>
#include <iostream>

// Sets default values
ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting up the root and mesh component of the boid
	BoidRootComp = CreateDefaultSubobject<USphereComponent>(TEXT("BoidRootComp"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidMesh"));

	SetRootComponent(BoidRootComp);
	Mesh->SetupAttachment(BoidRootComp);

	BoidRootComp->SetSimulatePhysics(true);
	BoidRootComp->SetEnableGravity(false);
	BoidRootComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoidRootComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BoidRootComp->BodyInstance.bLockRotation = true;
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
}

//Function for seeking nearby objects
FVector ABoid::Seek(FVector position)
{
	FVector newVelocity = position - GetActorLocation();
	newVelocity.Normalize();
	return newVelocity * Manager->seekPercent;
}

//Function for fleeing from nearby objects
FVector ABoid::Flee(FVector position)
{
	FVector newVelocity = position + GetActorLocation();
	newVelocity.Normalize();
	return newVelocity * Manager->fleePercent;
}

//Function to calculate a wander force
FVector ABoid::Wander(float radius, float distance, float jitter)
{
	FVector myLocation = GetActorLocation();
	if (FVector::Dist(myLocation, WanderDestination) < 100.0f) {
		FVector projectedPos = myLocation + (GetActorForwardVector() * distance);

		WanderDestination = projectedPos + (FMath::VRand() * FMath::RandRange(0.0f, jitter));
	}

	FVector jitterDestination = Seek(WanderDestination) + (FMath::VRand() * FMath::RandRange(0.0f, jitter));
	jitterDestination.Normalize();
	return jitterDestination;

	////Calculate the circle center 
	//FVector circleCenter;

	//circleCenter = currentForce;
	//circleCenter.Normalize();
	//circleCenter = circleCenter * circleDistance;
	// 
	////Calculate the displacement force 
	//FVector displacement;
	//displacement = currentVel;
	//displacement.Normalize();
	//displacement = displacement * circleDistance;
	//// 
	//// Randomly change the vector direction 
	//// by making it change its current angle 
	//int len = displacement.Length();
	//displacement.X = cos(wanderAngle) * len;
	//displacement.Y = sin(wanderAngle) * len;
	//// 
	//// Change wanderAngle just a bit, so it 
	//// won't have the same value in the 
	//// next game frame. 
	//wanderAngle += rand() * angleChange - angleChange * 0.5;
	//// 
	//// Finally calculate and return the wander force 
	//FVector wanderForce;
	//wanderForce = circleCenter + displacement;
	//return wanderForce;
}

//Function to improve the flee steering function
FVector ABoid::Evade(ABoidLeader* shark)
{
	if ((shark->GetActorLocation() - GetActorLocation()).Size() < 500) {
		FVector vNewVelocity = shark->GetActorLocation() + shark->GetVelocity() - GetActorLocation();
		FVector vForce = vNewVelocity * 5 - GetVelocity();
		vForce.Normalize();
		return Flee(vForce);
	}
	return FVector(0, 0.0, 0);
}

//Function to make boid avoid objects in the level
FVector ABoid::ObjectAvoidance()
{
	FHitResult hit;
	FVector awayForce = FVector(0, 0.0, 0);

	//Raycast infront of boid
	bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + GetVelocity() + 10, ECC_WorldStatic, FCollisionQueryParams(), FCollisionResponseParams());
	//if (Manager->debugLines == true) {
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + GetVelocity() + 10, 120.f, FColor::Magenta, false, 0.4f, 0, 5.f);
	//}
	if (actorHit && hit.GetActor()) 
	{
		//Checking that it isnt a boid
		if (hit.GetActor() != BoidActorClass)
		{
			awayForce = Flee(hit.GetActor()->GetActorLocation());
		}
	}
	awayForce.Normalize();
	return awayForce;
}

//Function to improve the seek function with a pursue
FVector ABoid::Pursue(ABoid* prey)
{
	FVector vNewVelocity = prey->GetActorLocation() + prey->GetVelocity() - GetActorLocation();
	FVector vForce = vNewVelocity * 5 - GetVelocity();
	vForce.Normalize();
	return Seek(vForce);
}

//Finding nearby boids
TArray<class ABoid*> ABoid::NearbyBoidPositions(ABoid* thisBoid, float range)
{
	//Setting up array
	TArray<class ABoid*> nearbyBoids;

	for (ABoid* boid : Manager->myBoids)
	{
		//Checking for a incorrect find
		if (boid == thisBoid || !boid)
		{
			continue;
		}

		//Calculating the distance between
		float aDistance = (boid->GetActorLocation() - thisBoid->GetActorLocation()).Size();

		if (aDistance < Manager->raduisOfDetectedBoids)
		{
			//Adding new boid to array
			nearbyBoids.Add(boid);
		}
	}
	return nearbyBoids;
}

//Seperation, alignment and cohesion
FVector ABoid::Separation()
{
	FVector directionAway = FVector::ZeroVector;
	int counter = 0;

	//Finding nearby boids
	TArray<class ABoid*> nearbyBoids = NearbyBoidPositions(this, Manager->raduisOfDetectedBoids);

	//Looping for all nearby boids
	for (ABoid* boid : nearbyBoids)
	{
		//Finding distance to other boid
		FVector directionFromOther = GetActorLocation() - boid->GetActorLocation();
		float distanceToOther = directionFromOther.Size();

		//If the distance is less than the seperation length then add to the direction away
		if (distanceToOther < Manager->separationLength)
		{
			if (distanceToOther == 0.f)
			{
				distanceToOther = 0.01f;
			}

			directionAway += directionFromOther;
			counter += 1;
		}
	}
	//Catching a potential error
	if (counter == 0)
	{
		return FVector::ZeroVector;
	}

	//Divided the direction away by how many boids affected the force
	directionAway /= counter;

	//Times normal value by percentage
	return directionAway.GetSafeNormal() * Manager->seperationPercent;
}

FVector ABoid::Alignment()
{
	FVector averageGroupVelocity = FVector::ZeroVector;

	//Finding nearby boids
	TArray<class ABoid*> nearbyBoids = NearbyBoidPositions(this, Manager->raduisOfDetectedBoids);

	//Catching a potential error
	if (nearbyBoids.Num() == 0)
	{
		return FVector::ZeroVector;
	}

	//Looping for all nearby boids
	for (ABoid* boid : nearbyBoids)
	{
		averageGroupVelocity += boid->GetVelocity().GetSafeNormal();
	}

	//Divided the direction of the group by how many boids affected the force
	averageGroupVelocity /= nearbyBoids.Num();
	averageGroupVelocity = averageGroupVelocity - GetActorLocation();

	//Times normal value by percentage
	return averageGroupVelocity.GetSafeNormal() * Manager->alignmentPercent;
}

FVector ABoid::Cohesion()
{
	FVector directionToMid = FVector::ZeroVector;
	FVector averagePos = FVector::ZeroVector;

	//Finding nearby boids
	TArray<class ABoid*> nearbyBoids = NearbyBoidPositions(this, Manager->raduisOfDetectedBoids);

	//Catching a potential error
	if (nearbyBoids.Num() == 0)
	{
		return FVector::ZeroVector;
	}

	//Looping for all nearby boids
	for (ABoid* boid : nearbyBoids)
	{
		averagePos += boid->GetActorLocation();
	}

	//Divided the direction to centre by how many boids affected the force
	averagePos /= nearbyBoids.Num();
	directionToMid = (averagePos - GetActorLocation());

	//Times normal value by percentage
	return directionToMid.GetSafeNormal() * Manager->cohesionPercent;
}

//Anchors the boid back to spawn point if it moves too far away
FVector ABoid::MoveTowardOrigin()
{
	FVector DirectionToSpawn = GetActorLocation() - Manager->GetActorLocation();
	float DistanceAway = DirectionToSpawn.Size();

	//Calculating the amount of force applied based on how much the boid is away from the border
	if (DistanceAway > Manager->MaxFleeDistance) {
		return DirectionToSpawn.GetSafeNormal() * -1;
	}
	else if (DistanceAway + 100 > Manager->MaxFleeDistance) {
		//return DirectionToSpawn.GetSafeNormal() * -0.1;
	}
	else {
		//return DirectionToSpawn.GetSafeNormal() * -0.01;
	}
	return FVector(0, 0.0, 0);
}

//Function to find other boids if the boid has gone too far away, works alongside wander
FVector ABoid::FindOtherBoids()
{
	ABoid* preyBoid = Manager->myBoids[0];
	float distance;
	float closestDistance = 1000000;

	//Checks for an error
	for (ABoid* boid : Manager->myBoids)
	{
		if (!boid)
		{
			continue;
		}
		//Calcualtes the distance and finds out whether its the closest
		distance = (boid->GetActorLocation() - GetActorLocation()).Size();
		if (distance < closestDistance)
		{
			closestDistance = distance;
			preyBoid = boid;
		}
	}
	FVector ToOtherBoids = Seek(preyBoid->GetActorLocation());
	return ToOtherBoids;
}

//Rotating the mesh to face its forward direction
void ABoid::RotateToFace()
{
	Mesh->SetWorldRotation(GetVelocity().Rotation() + (FRotator(-90.0f, 0.0f, 0.0f)));
}

//Setting up the mesh and manager of the boids
void ABoid::SetManagerAndMesh(ABoidManager* manager)
{
	Manager = manager;

	if (Manager != NULL)
	{
		Mesh->SetStaticMesh(Manager->BoidMesh);
	}
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Updating every boid every frame
	for (ABoid* boid : Manager->myBoids)
	{
		UpdateBoids(DeltaTime);
	}
}

void ABoid::UpdateBoids(float DeltaTime)
{
	FVector targetVelocity = FVector(0,0.0,0);

	//Performing a check after every addition to ensure that the max velocity isnt overstepped
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		FVector awayForce = ObjectAvoidance();
		targetVelocity += (awayForce * 0.1);
	}
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		if (Manager->containmentBounds) {
			FVector spawnForce = MoveTowardOrigin();
			targetVelocity += spawnForce;
		}
	}
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		FVector seperationForce = Separation();
		targetVelocity += seperationForce;
	}
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		FVector alignementForce = Alignment();
		targetVelocity += alignementForce;
	}
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		FVector cohesionForce = Cohesion();
		targetVelocity += cohesionForce;
	}
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		FVector wanderForce = Wander(10.0f, 20.0f, 5.0f);
		targetVelocity += (wanderForce * 0.1);
	}
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		//Only fleeing if close to the shark
		if ((Manager->leaderboid->GetActorLocation().Size() - GetActorLocation().Size()) < 500) {
			FVector fleeForce = Evade(Manager->leaderboid);
			targetVelocity += fleeForce;
		}
	}
	if (targetVelocity.Size() < Manager->maxVelocity)
	{
		//Only seeking nearby boids if it hasnt got any boid in its neighbourhood.
		if (NearbyBoidPositions(this, Manager->raduisOfDetectedBoids).Num() == 0) {
			FVector ToOtherBoids = FindOtherBoids();
			targetVelocity += ToOtherBoids;
		}
	}

	//Finding the force to apply
	targetVelocity *= Manager->speed * 20;
	currentForce = targetVelocity;

	RotateToFace();
	if (Manager->debugLines == true) {
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + (currentForce * 5), 120.f, FColor::Magenta, false, 0.4f, 0, 5.f);
	}

	//Adding the force
	BoidRootComp->AddForce(currentForce, NAME_None, true);
}
