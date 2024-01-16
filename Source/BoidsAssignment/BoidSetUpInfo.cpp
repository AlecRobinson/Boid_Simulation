// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidSetUpInfo.h"

// Sets default values
ABoidSetUpInfo::ABoidSetUpInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidSetUpInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoidSetUpInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

