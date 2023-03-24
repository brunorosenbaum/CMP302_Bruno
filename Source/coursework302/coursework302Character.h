// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "coursework302Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class Acoursework302Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	Acoursework302Character();
	virtual void Tick(float DeltaTime) override; 

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;



protected:
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	//Detect stakes inside radius
	void detectStakes();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	//Two functions needed: begin overlap and end overlap for the radius collider
	UFUNCTION() //Needs to have this tag if you want it to be accessed in the blueprint
		void beginOverlapRadius(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
	
	UFUNCTION()
		void endOverlapRadius(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex);
	
	//These functions are for the mouse press and its effect on raising the stakes
	void onPressMouse() { isMousePressed_ = true; 
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, TEXT("Mouse pressed"));
	}
	void onReleaseMouse() { isMousePressed_ = false; 
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, TEXT("Mouse released"));
	}


	
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	//Attributes
	bool isMousePressed_;
	float hp_;
	bool isRecoveryEnable_;

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	//Add sphere collider for the player to detect stakes around it
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* PlayerRadius; 

	void takeDamage() {
		hp_ -= 10.f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("HP: %f"), hp_));
	}
	//Toggling recovery for the action to be called via button press
	void toggleRecovery(){
		if (isRecoveryEnable_) {
			isRecoveryEnable_ = false;
		}
		else {
			isRecoveryEnable_ = true; 
		}
	}

};

