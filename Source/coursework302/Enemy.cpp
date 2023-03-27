// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "coursework302Character.h"
#include "coursework302Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Collider for detecting stakes
	EnemyCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Collision"));
	EnemyCollider->SetupAttachment(RootComponent); //Attach collider to root component cause every actor has a default one
	EnemyCollider->InitBoxExtent(FVector(50.0f, 50.f, 50.f));

	projectileSpawn = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn"));
	projectileSpawn->SetupAttachment(EnemyCollider);

	//Created skeletal mesh that will be used in the viewport
	mesh = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(EnemyCollider);
	enemyHp_ = 100.f;

	//Radius setup
	EnemyRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Radius enemy Collision"));
	EnemyRadius->SetupAttachment(EnemyCollider); //Attaching the collider to the player's mesh
	EnemyRadius->InitSphereRadius(400.f); //Add dimensions to it



	isFiring_ = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Attaching the collision functions to the collider
	EnemyRadius->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::beginOverlap);
	EnemyRadius->OnComponentEndOverlap.AddDynamic(this, &AEnemy::endOverlap);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (enemyHp_ <= 0.f) { //Destroy this enemy if its hp gets to this level
		Destroy();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Enemy HP: %f"), enemyHp_));

	if (isFiring_) {

		FVector targetLoc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(); //Takes camera's forward vector, multiplies it by a scalar (so it is projected)
		FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetLoc);
		FRotator newRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), targetRot, DeltaTime, 10.0f);
		SetActorRotation(newRot); 

		projectileTimer += DeltaTime;
		if (projectileTimer >= 1)
		{
			FRotator enRot = GetActorRotation();
			projectileTimer = 0;
			FActorSpawnParameters spawnParam;
			spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Acoursework302Projectile* proj = GetWorld()->SpawnActor<Acoursework302Projectile>(projectile, projectileSpawn->GetComponentLocation(), enRot, spawnParam);
		}
	}
}

void AEnemy::beginOverlap(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	//Check when you overlap radius collider with stake collider
	Acoursework302Character* mc_ = Cast<Acoursework302Character>(otherActor);
	if (IsValid(mc_)) { //Check if the ptr to this class is valid
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Player in enemy radius"));
		isFiring_ = true;
	}

}
void AEnemy::endOverlap(UPrimitiveComponent* overlapRadiusComp, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex) {
	//Check when you dont overlap radius collider with stake collider
	Acoursework302Character* mc_ = Cast<Acoursework302Character>(otherActor);
	if (IsValid(mc_)) { //Check if the ptr to this class is valid
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Player in enemy radius"));
		isFiring_ = false;

	}
}

