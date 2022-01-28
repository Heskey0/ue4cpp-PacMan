// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Enemy.h"
#include "../PacManGameModeBase.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override;

	void SearchNewPoint();

	void GoHome();
	void ReArm();
	void StopMove();

private:
	class AEnemy* ABot;
	FVector HomeLocation;

	FTimerHandle DeadTime;
	APacManGameModeBase* GameMode;

};
