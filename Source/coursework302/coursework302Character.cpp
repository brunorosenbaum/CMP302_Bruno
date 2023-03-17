// Copyright Epic Games, Inc. All Rights Reserved.

#include "coursework302Character.h"
#include "coursework302Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Stake.h"


//////////////////////////////////////////////////////////////////////////
// Acoursework302Character

Acoursework302Character::Acoursework302Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	//Radius sphere for detecting stakes
	PlayerRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Radius Collision"));
	PlayerRadius->SetupAttachment(GetCapsuleComponent()); //Attaching the collider to the player's mesh
	PlayerRadius->InitSphereRadius(400.f); //Add dimensions to it

}

void Acoursework302Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


		//Attaching the collision functions to the collider
	PlayerRadius->OnComponentBeginOverlap.AddDynamic(this, &Acoursework302Character::beginOverlapRadius);
	PlayerRadius->OnComponentEndOverlap.AddDynamic(this, &Acoursework302Character::endOverlapRadius);

}

void Acoursework302Character::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	detectStakes();
}

//////////////////////////////////////////////////////////////////////////// Input

void Acoursework302Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &Acoursework302Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &Acoursework302Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &Acoursework302Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &Acoursework302Character::LookUpAtRate);

	//Bind mouse bool functions to the mouse input
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &Acoursework302Character::onPressMouse);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &Acoursework302Character::onReleaseMouse);


}

void Acoursework302Character::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void Acoursework302Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void Acoursework302Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void Acoursework302Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void Acoursework302Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void Acoursework302Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void Acoursework302Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool Acoursework302Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &Acoursework302Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &Acoursework302Character::EndTouch);

		return true;
	}
	
	return false;
}

void Acoursework302Character::beginOverlapRadius(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	//Check when you overlap radius collider with stake collider
	AStake* stake_ = Cast<AStake>(otherActor);
	if (IsValid(stake_)) { //Check if the ptr to this class is valid
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Stake in range"));
		stake_->setWithinRadius(true);
	}
}
void Acoursework302Character::endOverlapRadius(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex) {
	//Check when you dont overlap radius collider with stake collider
	AStake* stake_ = Cast<AStake>(otherActor);
	if (IsValid(stake_)) { //Check if the ptr to this class is valid
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Stake out of range"));

		stake_->setWithinRadius(false);
		stake_->setTimerActive(false);
	}
}

void Acoursework302Character::detectStakes() {
	for (TObjectIterator<AStake> stake; stake; ++stake) { //Iterate through all stakes in the world
		if (stake->GetWorld() != GetWorld()) {//Check if it's in the same world as main character
			continue; //Move on to the next stake if not
		}
		//Check if the current one is within the radius of detection 
		if (stake->isWithinRadius()) {
			if (isMousePressed_) { //If the mouse is pressed, toggle bool for timer

				stake->setTimerActive(true);
				if (stake->isActive()) { //If three seconds of the timer have passed (active bool turns stake on active)
					if (stake->isReadyRot()) { //If the stake is ready to rotate
						FVector currentLoc = stake->GetTransform().GetLocation(); //Gets current location
						FVector targetLoc = GetActorLocation() + (FirstPersonCameraComponent->GetForwardVector() * 1000); //Takes camera's forward vector, multiplies it by a scalar (so it is projected)
						FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(currentLoc, targetLoc); //Finds out rotation angle between current location and the target (where you want it to rotate towards)
						//This is technically the dot product but this library does it for you. 
						stake->setTargetRotation(targetRot); 
						
					}
				}
			}
			else 
			{
				if (stake->isReadyRot()) {
					//if (stake->GetTransform().GetRotation().Rotator() == stake->getTargetRotation()) {
						stake->shootStake(); 
						stake->setShot(true);
					//}
				}
				stake->setTimerActive(false);

			}
		}
	}
}
