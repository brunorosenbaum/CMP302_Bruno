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
	
}

// Called every frame
void AStake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

