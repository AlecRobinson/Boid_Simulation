// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"
#include "BoidLeader.h"

// Sets default values
ABoidManager::ABoidManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this->SetRootComponent(transform);
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	//Spawning the boids and leader
	for (int i = 0; i < spawnCount; i++)
	{
		//Random location around the values given in main menu
		FVector spawnLocation = (FMath::VRand() * spawnRadius) + GetActorLocation();
		FRotator spawnRotation = GetActorRotation();

		ABoid* newboid = GetWorld()->SpawnActor<ABoid>(spawnLocation, spawnRotation);
		newboid->SetManagerAndMesh(this);
		myBoids.Add(newboid);
	}

	//Spawning one boid leader in the centre of world
	ABoidLeader* leader = GetWorld()->SpawnActor<ABoidLeader>(FVector(0, 1, 0), FRotator(0, 0, 0));
	leader->Manager = this;
	leaderboid = leader;

}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Removing the debug sphere from screen if bedug is turned off
	if (debugLines == true) {
		DrawDebugSphere(GetWorld(), GetActorLocation(), MaxFleeDistance, 26, FColor(181, 0, 0), false, 1, 0, 2);
	}
}

