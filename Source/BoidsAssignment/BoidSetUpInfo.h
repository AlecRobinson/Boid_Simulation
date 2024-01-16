// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidSetUpInfo.generated.h"

UCLASS()
class BOIDSASSIGNMENT_API ABoidSetUpInfo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidSetUpInfo();

	//SetUp
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SetUpSettings")
	int spawnCount = 60;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SetUpSettings")
	float spawnRadius = 1000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
