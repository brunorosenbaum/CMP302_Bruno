// Fill out your copyright notice in the Description page of Project Settings.


#include "Stake.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"


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
	//Set boolean values to all the inactive ones upon beginplay
	setTimerActive(false);
	setActive(false);
	setReadyRot(false);
	setShot(false);

	//Rotate 90 deg so the rotation method works as intended
	FRotator beginRot = GetTransform().GetRotation().Rotator();
	beginRot.Pitch -= 90.0f;
	SetActorRotation(beginRot); 
	startingPos = GetActorLocation();
	Collider->SetSimulatePhysics(false); //For protection - set them to false always before start of the game

	//Attaching the collision functions to the collider
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AStake::beginOverlapStake);
	

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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Rotation %f %f %f"),targetRotation.Pitch, targetRotation.Roll, targetRotation.Yaw));
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

void AStake::shootStake() {

	//Get a vector pointing in the direction of the stake's rotation
	FVector targetLoc = GetTransform().GetRotation().Vector();
	//Normalize it (needs to be to a value)
	targetLoc.Normalize(0.001);
	//And now multiply by a scalar to alter the projectile's velocity
	stakeMovement->Velocity = targetLoc * 2000; 
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Shootstake being called"));

}

void AStake::beginOverlapStake(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	//Check when you overlap radius collider with stake collider
	AEnemy* enemy_ = Cast<AEnemy>(otherActor);
	if (IsValid(enemy_)) { //Check if the ptr to this class is valid
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Stake colliding with enemy"));
		float newEnemyHP = enemy_->getenemyHP() - 20.f;
		enemy_->setEnemyHP(newEnemyHP);
	}
}
