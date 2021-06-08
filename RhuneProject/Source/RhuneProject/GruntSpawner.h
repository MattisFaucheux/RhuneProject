// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ActorCamp.h"

#include "GameFramework/Actor.h"
#include "GruntSpawner.generated.h"

class AFortress;

UCLASS()
class RHUNEPROJECT_API AGruntSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGruntSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
		bool isActivate = true;

	/* Actual Camp of the fortress linked tot he spawner*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
		TEnumAsByte<ActorCamp> ActualCamp;
	
	UPROPERTY(EditAnywhere, Category = Spawner)
		TArray<TSubclassOf<class AActor>> SpawnedActorTypeEnemy;

	UPROPERTY(EditAnywhere, Category = Spawner)
		TArray < TSubclassOf<class AActor>> SpawnedActorTypeAlly;

	/* Number of waves, if = 0 -> infinite number of waves*/
	UPROPERTY(EditAnywhere, Category = Spawner, meta = (ClampMin = "0.0"))
		int NbrOfWaves = 1;

	/* Number of enemies that will spawn at each waves if SpawnPerWave = true*/
	UPROPERTY(EditAnywhere, Category = Spawner)
		TArray<int> NbrEnemyPerWave;

	/* Radius to spawn arround the spawner*/
	UPROPERTY(EditAnywhere, Category = Spawner, meta = (ClampMin = "0.0"))
		float Radius = 250.f;

	/* Time to wait before the first spawn in seconds */
	UPROPERTY(EditAnywhere, Category = Spawner, meta = (ClampMin = "0.0"))
		float TimeBeforeFirstSpawn = 0.f;

	/* frequency between spawn in seconds or frequency between waves */
	UPROPERTY(EditAnywhere, Category = Spawner, meta = (ClampMin = "0.1"))
		float Frequency = 1.f;

	/*Fortress Target for grunt*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
		AFortress* FortressTarget;


public:

	UFUNCTION(BlueprintCallable)
		void SetSpawnerCamp(TEnumAsByte<ActorCamp> FortressCamp);

private:
	
	void SpawnWaveOfActors();

	FTimerHandle SpawnTimer;
	
	int NbrOfWavesMade = 0;

};
