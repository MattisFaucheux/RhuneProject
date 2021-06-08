// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"


#include "Fortress.h"
#include "ActorCamp.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"


AGameManager* AGameManager::MS_Instance = nullptr;

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AGameManager::~AGameManager()
{
	if(MS_Instance == this)
	{
		MS_Instance = nullptr;
	}
}

void AGameManager::StartGame()
{
	IsGameStart = true;
	GetWorldTimerManager().SetTimer(RefreshGameStatusTimer, this, &AGameManager::UpdateGameStatus, RefreshGameStatusFrequency, true, 0.f);

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	if(MS_Instance == nullptr)
	{
		MS_Instance = this;
	}
	else
	{
		Destroy();
	}
	
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(StartGameTimer, this, &AGameManager::StartGame, 1000.f, false, TimeBeforeStart);
}


bool AGameManager::IsWin()
{
	return IsGameWin;
}

bool AGameManager::IsLost()
{
	return IsGameLost;
}

bool AGameManager::IsStart()
{
	return IsGameStart;
}

void AGameManager::UpdateGameStatus()
{
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FortressTag, ActorsWithTag);

	if (ActorsWithTag.Num() == 0)
	{
		return;
	}

	AActor* AITarget = nullptr;

	bool IsAllyFortress = false;
	bool IsEnemyFortress = false;

	for (int TargetIndex = 0; TargetIndex < ActorsWithTag.Num(); TargetIndex++)
	{
		AITarget = ActorsWithTag[TargetIndex];

		if (AITarget->IsA<AFortress>())
		{
			AFortress* Fortress = Cast<AFortress>(AITarget);
			if (Fortress->IsSameCamp(ActorCamp::EnemyActor))
			{
				IsEnemyFortress = true;
			}
			else 
			{
				IsAllyFortress = true;
			}
		}
	}

	if (IsAllyFortress && IsEnemyFortress)
	{
		return;
	}
	else if(IsAllyFortress) 
	{
		IsGameWin = true;
	}
	else if(IsEnemyFortress) 
	{
		IsGameLost = true;
	}
}

AGameManager* AGameManager::GetInstance()
{
	return MS_Instance;
}

