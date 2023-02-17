// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h" //For adding a spheric collision into the world - need this header
#include "Stake.generated.h"

UCLASS()
class COURSEWORK302_API AStake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStake();
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* stakeMesh;

protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //Tag for the variable - so can be edited in the blueprint
		class UBoxComponent* Collider; //Creating a box collision range
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Getters and setters for functionality
	void setWithinRadius(bool s) { isWithinRadius_ = s; }
	bool isWithinRadius() { return isWithinRadius_; }

protected: //Variables for functionality
	bool isWithinRadius_; 
};
