// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.generated.h"

UCLASS()
class PACMAN_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,Category=Body)
	UStaticMeshComponent* EnemyBody;

	void SetGray();
	void SetDefault();

	void SetMove(bool bMoveIt);
	void Killed();
	void ReArm();	//�������ɵ���ʱ���ó�ʼ�ٶ�
	bool bIsDead;

	UFUNCTION()
		void OnCollision(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	class UMaterialInterface* DefaultMaterial;
	class UMaterialInterface* GrayMaterial;

	FTimerHandle TimeGray;
	bool bIsGray;
};
