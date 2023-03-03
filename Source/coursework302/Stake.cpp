// Fill out your copyright notice in the Description page of Project Settings.


#include "Stake.h"

// Sets default values
AStake::AStake()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Attacking Collision"));
	//Mesh - create mesh and collider
	stakeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StakeMesh"));
	stakeMesh->SetupAttachment(Collider);

	//Create definition of movement component to be manipulated in the editor
	//stakeMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Stake Movement"));
	//stakeMovement->ProjectileGravityScale = 0;
	//stakeMovement->Deactivate(); 
	heightOffset = 0.0f;
}

// Called when the game starts or when spawned
void AStake::BeginPlay()
{
	Super::BeginPlay();
	setWithinRadius(false); //Default should be false unless colliding with player's radius
	setTimerActive(false);
	setActive(false);
	startingPos = GetActorLocation();

}

// Called every frame
void AStake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isTimerActive()) {
		timer_ += DeltaTime;
	}
	else {
		timer_ = 0.0f;
		setActive(false);
	}

	//Check if timer > 3 seconds, activate a bool
	if (timer_ > 3.0f) {

		setActive(true);
	}


	//Condition for levitating the stake object
	if (isActive()) {
		heightOffset += DeltaTime * movementScalar;
		if (heightOffset > 100.f) {
			heightOffset = 100.f;
		}

		FVector newLocation = FVector(startingPos.X, startingPos.Y, startingPos.Z + heightOffset);
		SetActorLocation(newLocation);

	}
}

