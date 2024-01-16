// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidLeader.generated.h"

UCLASS()
class BOIDSASSIGNMENT_API ABoidLeader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidLeader();

	FVector currentForce = FVector::ZeroVector;
	class ABoidManager* Manager;
	class ABoid* preyBoid;

	//Functions for shark movement
	FVector Seek(FVector position);
	FVector Pursue();
	FVector MoveTowardOrigin();
	class ABoid* ClosePrey(float range);

	UFUNCTION(BlueprintCallable)
	class ABoid* ChangePrey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Properties for visuals on shark
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* RootComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USceneComponent* MeshParent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
