// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class RHUNEPROJECT_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	~AGameManager();

private:

	FTimerHandle RefreshGameStatusTimer;

	FTimerHandle StartGameTimer;

	static AGameManager* MS_Instance;

	void StartGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameState)
		bool IsGameWin = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameState)
		bool IsGameLost = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameState)
		bool IsGameStart = false;

	UPROPERTY(EditDefaultsOnly, Category = GameState)
		FName FortressTag = "AITarget";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
		float RefreshGameStatusFrequency = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
		float TimeBeforeStart = 30.f;

	UFUNCTION(BlueprintCallable)
		void UpdateGameStatus();

public:

	UFUNCTION(BlueprintCallable)
		bool IsWin();

	UFUNCTION(BlueprintCallable)
		bool IsLost();

	UFUNCTION(BlueprintCallable)
		bool IsStart();

	UFUNCTION(BlueprintCallable)
	static AGameManager* GetInstance();

};
