// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidsBlueprintFunctionLibrary.h"
#include "BoidManager.h"


void UBoidsBlueprintFunctionLibrary::TestFunction(FString SomeString)
{
	UE_LOG(LogTemp, Log, TEXT("You said %s"), *SomeString);

}

float UBoidsBlueprintFunctionLibrary::ChangeCohesion(float newCohesionPercent)
{
	//Manager->cohesionPercent = newCohesionPercent;
	return newCohesionPercent;
}

float UBoidsBlueprintFunctionLibrary::ChangeSeperation(float seperationPercent)
{
	float newSeperationPercent = seperationPercent;
	return newSeperationPercent;
}

float UBoidsBlueprintFunctionLibrary::ChangeAlignment(float alignmentPercent)
{
	float newAlignmentPercent = alignmentPercent;
	return newAlignmentPercent;
}
