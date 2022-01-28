// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Public/PacManCharacter.h"
#include "../Public/EnemyAIController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderObj(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));

	if (CylinderObj.Succeeded())
	{
		EnemyBody->SetStaticMesh(CylinderObj.Object);
	}

	EnemyBody->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.8f));
	EnemyBody->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(50.0f);

	static ConstructorHelpers::FObjectFinder<UMaterial> GrayMaterialFinder(TEXT("'/Game/Material/Enemy/M_Gray'"));
	if (GrayMaterialFinder.Succeeded())
	{
		GrayMaterial = GrayMaterialFinder.Object;
	}

	AIControllerClass = AEnemyAIController::StaticClass();
	SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultMaterial = EnemyBody->GetMaterial(0);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCollision);
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetGray()
{
	GetWorldTimerManager().SetTimer(TimeGray, this, &AEnemy::SetDefault, 10.0f, false);
	if (bIsGray) {
		return;
	}
	bIsGray = true;
	EnemyBody->SetMaterial(0, GrayMaterial);
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
}

void AEnemy::SetDefault()
{
	GetWorldTimerManager().ClearTimer(TimeGray);
	bIsGray = false;
	EnemyBody->SetMaterial(0, DefaultMaterial);
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void AEnemy::SetMove(bool bMoveIt)
{
	AEnemyAIController* AI= Cast<AEnemyAIController>(GetController());

	if (bMoveIt)
	{
		AI->SearchNewPoint();
	}
	else {
		AI->StopMove();
	}
}

void AEnemy::Killed()
{
	if (bIsDead) {
		return;
	}
	bIsDead = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;	//TODO:
	AEnemyAIController* AI = Cast<AEnemyAIController>(GetController());
	AI->GoHome();
}

void AEnemy::ReArm()
{
	bIsDead = false;
	GetCharacterMovement()->MaxWalkSpeed = 150;
	if (bIsGray)
	{
		SetDefault();
	}
}

void AEnemy::OnCollision(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(APacManCharacter::StaticClass())) {
		if (bIsGray) {
			Killed();
		}
		else {
			APacManCharacter* PacMan = Cast<APacManCharacter>(OtherActor);
			PacMan->Attacked();
		}
	}
}
