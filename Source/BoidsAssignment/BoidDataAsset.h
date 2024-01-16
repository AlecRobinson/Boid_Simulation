// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoidDataAsset.generated.h"

/**
 *
 */
UCLASS()
class BOIDSASSIGNMENT_API UBoidDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	//Flocking
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats", meta = (UIMin = "0.0", UIMax = "1.0"))
		float cohesionPercent = 0.25f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats") //meta = (UIMin = "0.0", UIMax = "1.0"))
		float separationLength = 300.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats", meta = (UIMin = "0.0", UIMax = "1.0"))
		float seperationPercent = 0.4f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats", meta = (UIMin = "0.0", UIMax = "1.0"))
		float alignmentPercent = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats", meta = (UIMin = "0.0", UIMax = "1.0"))
		float seekPercent = 0.05f;


};
