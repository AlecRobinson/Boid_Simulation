// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidLeader.h"
#include "BoidManager.h"
#include "Boid.h"
#include "Components/SphereComponent.h"

// Sets default values
ABoidLeader::ABoidLeader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting up the root and mesh component of the boid
	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidMesh"));

	SetRootComponent(RootComp);
	Mesh->SetupAttachment(RootComp);

	RootComp->SetSimulatePhysics(true);
	RootComp->SetEnableGravity(false);
	RootComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComp->BodyInstance.bLockRotation = true;

	UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	Mesh->SetStaticMesh(sphereMesh);

}

// Called when the game starts or when spawned
void ABoidLeader::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoidLeader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector targetVelocity = FVector(0, 0.0, 0);
	
	//FVector seekForce = Pursue();			//I turned off pursue to make the shark more dumb so that the boids could ecsape more often
	FVector seekForce = Seek(ClosePrey(Manager->raduisOfDetectedBoids)->GetActorLocation());
	FVector homeForce = MoveTowardOrigin();

	targetVelocity = seekForce + homeForce;

	//Finding the force to apply
	targetVelocity *= Manager->sharkSpeed * 50;
	currentForce = targetVelocity;

	if (Manager->debugLines == true) {
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + (currentForce), 120.f, FColor::Magenta, false, 0.4f, 0, 5.f);
	}

	//Speeding it up if the shark is going in the wrong direction
	if (currentForce.DotProduct(currentForce, GetVelocity()) < 0.1) {
		currentForce *= 1.4;
	}

	//Adding the force
	RootComp->AddForce(currentForce, NAME_None, true);
}

//Function for seeking nearby objects
FVector ABoidLeader::Seek(FVector position)
{
	FVector newVelocity = position - GetActorLocation();
	return newVelocity.GetSafeNormal() * Manager->seekPercent;
}

//Function to improve the seek function with a pursue
FVector ABoidLeader::Pursue()
{
	preyBoid = Manager->myBoids[0];
	if (FMath::RandRange(0, 1000) == 1) {
		preyBoid = ClosePrey(Manager->raduisOfDetectedBoids);
	}
	FVector vNewVelocity = preyBoid->GetActorLocation() + preyBoid->GetVelocity() - GetActorLocation();
	FVector vForce = vNewVelocity * 5 - GetVelocity();
	return Seek(vForce);
}

//Anchors the shark back to spawn point if it moves too far away
FVector ABoidLeader::MoveTowardOrigin()
{
	FVector DirectionToSpawn = GetActorLocation() - Manager->GetActorLocation();
	float DistanceAway = DirectionToSpawn.Size();

	//Calculating the amount of force applied based on how much the boid is away from the border
	if (DistanceAway > Manager->MaxFleeDistance) {
		return DirectionToSpawn.GetSafeNormal() * -1;
	}
	else if (DistanceAway + 100 > Manager->MaxFleeDistance) {
		return DirectionToSpawn.GetSafeNormal() * -0.1;
	}
	return FVector(0, 0.0, 0);
}

//Finds the clostest boid and sets it as the prey
class ABoid* ABoidLeader::ClosePrey(float range)
{
	//Presets the variables
	preyBoid = Manager->myBoids[0];
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
	return preyBoid;
}

//Function to change the prey when using pursue
ABoid* ABoidLeader::ChangePrey()
{
	preyBoid = ClosePrey(Manager->raduisOfDetectedBoids);
	return preyBoid;
}

