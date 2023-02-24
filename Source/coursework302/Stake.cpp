// Fill out your copyright notice in the Description page of Project Settings.


#include "Stake.h"

// Sets default values
AStake::AStake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Attacking Collision"));
	//Mesh
	stakeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StakeMesh"));
	stakeMesh->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void AStake::BeginPlay()
{
	Super::BeginPlay();
	setWithinRadius(false); //Default should be false unless colliding with player's radius
	setTimerActive(false);
	setActive(false); 
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
	
}

