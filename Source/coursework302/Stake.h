// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h" //For adding a spheric collision into the world - need this header
#include "GameFramework/ProjectileMovementComponent.h"
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

	// Called for shooting
	void shootStake();

	//Getters and setters for functionality
	void setWithinRadius(bool s) { isWithinRadius_ = s; }
	bool isWithinRadius() { return isWithinRadius_; }

	void setActive(bool a) { isActive_ = a; }
	bool isActive() { return isActive_; }

	void setTimerActive(bool t) { isTimerActive_ = t; }
	bool isTimerActive() { return isTimerActive_; }

	void setTargetRotation(FRotator rot) { targetRotation = rot; }
	FRotator getTargetRotation() { return targetRotation; }

	void setReadyRot(bool r) { isReadyRot_ = r; }
	bool isReadyRot() { return isReadyRot_; }

	void setShot(bool s) { isShot_ = s; }
	bool isShot() { return isShot_; }

	void setGravityScale(float scale)
	{
		stakeMovement->ProjectileGravityScale = scale;
	}
	//Two functions needed: begin overlap and end overlap for the radius collider
	UFUNCTION() //Needs to have this tag if you want it to be accessed in the blueprint
		void beginOverlapStake(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	

protected: //Variables for functionality
	bool isWithinRadius_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //Tag for the variable - so can be edited in the blueprint
		bool isActive_;
	bool isTimerActive_;

	bool isReadyRot_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //Tag for the variable - so can be edited in the blueprint
		bool isShot_;
	float timer_ = 0.0f;
	float rotTimer_ = 0.0f;

	FRotator targetRotation;

	FVector startingPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //Tag for the variable - so can be edited in the blueprint
		float heightOffset; //Will refer to how much we need to levitate this

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //Tag for the variable - so can be edited in the blueprint
		float movementScalar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //Tag for the variable - so can be edited in the blueprint
		UProjectileMovementComponent* stakeMovement;

};
