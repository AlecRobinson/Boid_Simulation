// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BoidsBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BOIDSASSIGNMENT_API UBoidsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "BoidsUI")
	static void TestFunction(FString SomeString);

	class ABoidManager* Manager;

	UFUNCTION(BlueprintCallable, Category = "BoidsUI")
	static float ChangeCohesion(float cohesionPercent);
	UFUNCTION(BlueprintCallable, Category = "BoidsUI")
	static float ChangeSeperation(float seperationPercent);
	UFUNCTION(BlueprintCallable, Category = "BoidsUI")
	static float ChangeAlignment(float alignmentPercent);
};
