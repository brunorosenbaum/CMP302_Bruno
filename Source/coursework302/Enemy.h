// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy.generated.h"

UCLASS()
class COURSEWORK302_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* EnemyCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* projectileSpawn; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Two functions needed: begin overlap and end overlap for the radius collider
	UFUNCTION() //Needs to have this tag if you want it to be accessed in the blueprint
		void beginOverlap(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	UFUNCTION()
		void endOverlap(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex);

public:	
	// Called every frame

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class Acoursework302Projectile> projectile;
	//Add sphere collider for the enemy to detect if the player is around it
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USphereComponent* EnemyRadius;



	virtual void Tick(float DeltaTime) override;

	float getenemyHP() { return enemyHp_; }
	void setEnemyHP(float hp) { enemyHp_ = hp; }

protected:
	//Attributes
	float enemyHp_; 
	bool isHit_; 
	bool isFiring_; 
	float projectileTimer; 


};
