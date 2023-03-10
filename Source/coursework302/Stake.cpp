// Fill out your copyright notice in the Description page of Project Settings.


#include "Stake.h"
#include "Kismet/KismetMathLibrary.h"

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
	stakeMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Stake Movement"));
	stakeMovement->ProjectileGravityScale = 0;
	stakeMovement->Deactivate();
	heightOffset = 0.0f;
	rotTimer_ = 0.0f; 
}

// Called when the game starts or when spawned
void AStake::BeginPlay()
{
	Super::BeginPlay();
	setWithinRadius(false); //Default should be false unless colliding with player's radius
	setTimerActive(false);
	setActive(false);
	setReadyRot(false);

	startingPos = GetActorLocation();
	Collider->SetSimulatePhysics(false); //For protection - set them to false always before start of the game

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
	if (isReadyRot()) {
		FRotator currentRot = GetTransform().GetRotation().Rotator(); 

		FRotator newRot = UKismetMathLibrary::RInterpTo(currentRot, targetRotation, DeltaTime, 10.0f);
		SetActorRotation(newRot); 
	}

	//Condition for levitating the stake object
	if (isActive()) //Is active once mouse is pressed for > 3 sec
	{
		//GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Cyan, TEXT("Active"));

		stakeMovement->ProjectileGravityScale = 0; //Gravity is 0 when it's on the ground 
		
		heightOffset += DeltaTime * movementScalar; //Add a height offset so it's added to the z axis of the object every frame

		if (heightOffset > 200.f) { //Cap it at 200
			heightOffset = 200.f;
			rotTimer_ += DeltaTime;
			if (rotTimer_ > 1.0f) { //If one second has elapsed
				setReadyRot(true); 
			}
		}
		//Update actors location by adding the offset
		FVector newLocation = FVector(startingPos.X, startingPos.Y, startingPos.Z + heightOffset); 
		SetActorLocation(newLocation);
	}
	else //Mouse is released
	{
		//GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Cyan, TEXT("Not Active"));

		if (heightOffset > 0.0f) { //If it's lifted off the ground
			stakeMovement->ProjectileGravityScale = 1; //Add gravity
			stakeMovement->SetUpdatedComponent(RootComponent); //Reset the component so it can be modified again
			stakeMovement->Activate(); 
			heightOffset = 0.0f; //Reset here bc it doesnt get decreased outside of it
		}
		startingPos = GetTransform().GetLocation(); //Reset starting pos
		setReadyRot(false); 
		rotTimer_ = 0.0f; 
	}
}

