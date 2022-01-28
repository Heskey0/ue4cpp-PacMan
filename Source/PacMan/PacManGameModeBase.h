// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enemy.h"
#include "PacManGameModeBase.generated.h"

/**
 * 
 */

enum class EGameState :short {
	EMenu,
	EPlaying,
	EPause,
	EWin,
	EGameOver,
};

UCLASS()
class PACMAN_API APacManGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	EGameState GetCurrentState() const;

	void SetCurrentState(EGameState value);

	void BeginPlay();

	void SetEnemyGray();


private:
	EGameState currentState;
	TArray<AEnemy*> Enemys;

};

FORCEINLINE EGameState APacManGameModeBase::GetCurrentState() const {
	return currentState;
};
