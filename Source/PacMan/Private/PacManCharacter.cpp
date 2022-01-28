// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Collectables.h"
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h"

// Sets default values
APacManCharacter::APacManCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacManCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameMode =Cast<APacManGameModeBase>(UGameplayStatics::GetGameMode(this));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacManCharacter::OnCollision);

	StartPoint = GetActorLocation();
	Lives = 3;
	for (TActorIterator<ACollectables> CollectableItr(GetWorld()); CollectableItr; ++CollectableItr) {
		CollectablesToEat++;
	}
	//CollectablesToEat = 100;
	UE_LOG(LogTemp, Warning, TEXT("Total Collectables is %d"), CollectablesToEat);
}

// Called every frame
void APacManCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveXAxis", this, &APacManCharacter::MoveXAxis);
	PlayerInputComponent->BindAxis("MoveYAxis", this, &APacManCharacter::MoveYAxis);

	PlayerInputComponent->BindAction("ReStart",IE_Pressed, this, &APacManCharacter::ReStart);
	PlayerInputComponent->BindAction("Pause",IE_Pressed, this, &APacManCharacter::Pause);
	PlayerInputComponent->BindAction("NewGame", IE_Pressed, this, &APacManCharacter::NewGame);

}

void APacManCharacter::MoveXAxis(float AxisValue)
{
	if (GameMode->GetCurrentState() != EGameState::EPlaying) {
		return;
	}
	CurrentVelocity.X = AxisValue;
	AddMovementInput(CurrentVelocity);
}

void APacManCharacter::MoveYAxis(float AxisValue)
{
	if (GameMode->GetCurrentState() != EGameState::EPlaying) {
		return;
	}
	CurrentVelocity.Y = AxisValue;
	AddMovementInput(CurrentVelocity);
}

void APacManCharacter::ReStart()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

void APacManCharacter::NewGame()
{
	UE_LOG(LogTemp, Warning, TEXT("NewGame"));
	if (GameMode->GetCurrentState()==EGameState::EMenu)
	{
		GameMode->SetCurrentState(EGameState::EPlaying);
	}
}

void APacManCharacter::Pause()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause"));
	if (GameMode->GetCurrentState() == EGameState::EPlaying) {
		GameMode->SetCurrentState(EGameState::EPause);
	}
	else if(GameMode->GetCurrentState()==EGameState::EPause)
	{
		GameMode->SetCurrentState(EGameState::EPlaying);
	}
}

void APacManCharacter::Attacked()
{
	if (--Lives <= 0) {
		GameMode->SetCurrentState(EGameState::EGameOver);
	}
	else
	{
		SetActorLocation(StartPoint);
	}
}

void APacManCharacter::OnCollision(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GameMode->GetCurrentState() == EGameState::EPlaying) {
		if (OtherActor->IsA(ACollectables::StaticClass()))
		{
			ACollectables* collectable=Cast<ACollectables>(OtherActor);

			if (collectable->bIsSuperCollectable) {
				GameMode->SetEnemyGray();
			}

			OtherActor->Destroy();
			if (--CollectablesToEat<=0)
			{
				GameMode->SetCurrentState(EGameState::EWin);
			}
		}
	}
}

