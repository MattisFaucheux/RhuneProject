// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntSpawner.h"


#include "AIController.h"
#include "BasicGruntCharacter.h"
#include "Fortress.h"

// Sets default values
AGruntSpawner::AGruntSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGruntSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AGruntSpawner::SpawnWaveOfActors, Frequency, false, TimeBeforeFirstSpawn);
}

void AGruntSpawner::SetSpawnerCamp(TEnumAsByte<ActorCamp> FortressCamp)
{
	NbrOfWavesMade = 0;
	
	ActualCamp = FortressCamp;

	GetWorldTimerManager().ClearTimer(SpawnTimer);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AGruntSpawner::SpawnWaveOfActors, Frequency, false, TimeBeforeFirstSpawn);
}

// Called every frame
void AGruntSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FortressTarget != nullptr)
	{
		if (FortressTarget->IsSameCamp(ActualCamp) && isActivate)
		{
			isActivate = false;
			NbrOfWavesMade = 0;
			GetWorldTimerManager().ClearTimer(SpawnTimer);
		}
		else if (!FortressTarget->IsSameCamp(ActualCamp) && !isActivate)
		{
			isActivate = true;
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AGruntSpawner::SpawnWaveOfActors, Frequency, false, TimeBeforeFirstSpawn);
		}
	}
}

void AGruntSpawner::SpawnWaveOfActors()
{
	if (NbrEnemyPerWave.Num() == 0 || !isActivate)
	{
		return;
	}

	UWorld* MyWorld = GetWorld();
	if (MyWorld == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int IndexEnemiesType = 0; IndexEnemiesType < NbrEnemyPerWave.Num(); IndexEnemiesType++)
	{

		for (int NbrOfEnemiesSpawn = 0; NbrOfEnemiesSpawn < NbrEnemyPerWave[IndexEnemiesType]; NbrOfEnemiesSpawn++)
		{
			FVector2D RandomPoint = FMath::RandPointInCircle(Radius);
			FVector SpawnPoint = GetActorLocation() + FVector(RandomPoint, 0);

			AActor* MyNewActor = nullptr;

			if (ActualCamp == ActorCamp::EnemyActor)
			{
				if (SpawnedActorTypeEnemy.Num() == 0)
				{
					return;
				}

				if (SpawnedActorTypeEnemy[IndexEnemiesType] == nullptr)
				{
					break;
				}

				MyNewActor = MyWorld->SpawnActor<AActor>(SpawnedActorTypeEnemy[IndexEnemiesType], SpawnPoint, FRotator::ZeroRotator, SpawnParams);
			}
			else if (ActualCamp == ActorCamp::AllyActor)
			{
				if (SpawnedActorTypeAlly.Num() == 0)
				{
					return;
				}

				if (SpawnedActorTypeAlly[IndexEnemiesType] == nullptr)
				{
					break;
				}

				MyNewActor = MyWorld->SpawnActor<AActor>(SpawnedActorTypeAlly[IndexEnemiesType], SpawnPoint, FRotator::ZeroRotator, SpawnParams);

			}
		}
	}

	NbrOfWavesMade += 1;

	if(NbrOfWaves == 0 || NbrOfWavesMade < NbrOfWaves)
	{
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AGruntSpawner::SpawnWaveOfActors, Frequency, false);
	}
}

