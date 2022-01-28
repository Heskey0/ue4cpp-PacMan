// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManGameModeBase.h"
#include "EngineUtils.h"

void APacManGameModeBase::SetCurrentState(EGameState value)
{
	currentState = value;

	switch (value)
	{
	case EGameState::EPlaying:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++) {
			(*Iter)->SetMove(true);
		}
		break;
	case EGameState::EPause:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++) {
			(*Iter)->SetMove(false);
		}
		break;
	case EGameState::EWin:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++) {
			(*Iter)->Destroy();
		}
		break;
	case EGameState::EGameOver:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++) {
			(*Iter)->Destroy();
		}
		break;
	default:
		break;
	}
}

void APacManGameModeBase::BeginPlay() {
	SetCurrentState(EGameState::EMenu);

	for (TActorIterator<AEnemy> enemyItr(GetWorld()); enemyItr; ++enemyItr) {
		AEnemy* enemy = Cast<AEnemy>(*enemyItr);
		if (enemy)
		{
			Enemys.Add(enemy);
		}
	}
}

void APacManGameModeBase::SetEnemyGray()
{
	for (auto Iter(Enemys.CreateIterator()); Iter; Iter++) {
		(*Iter)->SetGray();
	}
}
