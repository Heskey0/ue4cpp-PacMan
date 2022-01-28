// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectables.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACollectables::ACollectables()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	//Tick是否每帧调用
	SetActorEnableCollision(true);			//启用碰撞

	CollectableMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectableMeshComponent"));
	BaseCollisionCompnent = CreateDefaultSubobject<USphereComponent>(TEXT("BaseCollisionComponent"));
	CollectableMeshComponent->SetupAttachment(BaseCollisionCompnent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		CollectableMeshComponent->SetStaticMesh(Sphere.Object);
	}
	CollectableMeshComponent->SetWorldScale3D(FVector(0.3, 0.3, 0.3));
	BaseCollisionCompnent->SetSphereRadius(16);
}

// Called when the game starts or when spawned
void ACollectables::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

