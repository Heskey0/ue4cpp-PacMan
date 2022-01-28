// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void AEnemyAIController::OnPossess(APawn * InPawn) {
	Super::OnPossess(InPawn);
	ABot = Cast<AEnemy>(InPawn);
	HomeLocation = ABot->GetActorLocation();
	GameMode = Cast<APacManGameModeBase>(UGameplayStatics::GetGameMode(this));

	SearchNewPoint();
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	if (!ABot->bIsDead&&GameMode->GetCurrentState()!=EGameState::EPause)
	{
		SearchNewPoint();
	}
}

void AEnemyAIController::SearchNewPoint()
{
	UNavigationSystemV1* NavMesh = UNavigationSystemV1::GetCurrent(this);

	if (NavMesh)
	{
		const float SearchRadius = 10000.0f;
		FNavLocation RandomPt;

		const bool bFound = NavMesh->GetRandomReachablePointInRadius(ABot->GetActorLocation(),SearchRadius,RandomPt);
	
		if (bFound) {
			MoveToLocation(RandomPt);
		}
	}

}

void AEnemyAIController::GoHome()
{
	MoveToLocation(HomeLocation);
	GetWorldTimerManager().SetTimer(DeadTime, this, &AEnemyAIController::ReArm, 50.f, false);
}

void AEnemyAIController::ReArm()
{
	GetWorldTimerManager().ClearTimer(DeadTime);
	ABot->ReArm();
}

void AEnemyAIController::StopMove()
{
	//StopMovement();
	MoveToLocation(ABot->GetActorLocation());
}
