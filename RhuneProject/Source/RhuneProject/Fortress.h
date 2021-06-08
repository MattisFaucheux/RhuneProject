// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ActorCamp.h"

#include "GameFramework/Actor.h"
#include "Fortress.generated.h"

class UHpComponent;
class AGruntSpawner;

UCLASS()
class RHUNEPROJECT_API AFortress : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFortress();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		bool IsSameCamp(TEnumAsByte<ActorCamp> OtherFortressCamp);

private:

	FTimerHandle HealLoopTimer;

	FTimerHandle HealRestartLoopTimer;
	
protected:

	UFUNCTION(BlueprintCallable)
		void TakeDamages(float damages);

	UFUNCTION(BlueprintCallable)
		void SwitchCamp();

	UFUNCTION(BlueprintCallable)
		float GetNumberOfHp();

	UFUNCTION(BlueprintCallable)
		float GetNumberOfHpMax();

	UFUNCTION(BlueprintCallable)
		void SetSpawnerCamp(TEnumAsByte<ActorCamp> FortressCamp);

	UFUNCTION(BlueprintCallable)
		void HealFortress();

	UFUNCTION(BlueprintCallable)
		void StartHealLoop();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh = nullptr;
	
	/* Hp Component*/
	UPROPERTY(VisibleAnywhere, Category = Hp)
		class UHpComponent* HpComponent;

	/* Actual Camp of the fortress*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpecialParameters)
		TEnumAsByte<ActorCamp> ActualFortressCamp;

	/* Enemy spawners linked to the fortress*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Parameters")
		TArray<AGruntSpawner*>  EnemiesSpawner;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Special Parameters\|Regeneration", meta = (ClampMin = 0.0))
		float TimeWithoutHitToRegenerate = 15.f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Special Parameters\|Regeneration", meta = (ClampMin = 0.1))
		float FrequencyOfHeal = 2.5f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Special Parameters\|Regeneration", meta = (ClampMin = 0.0))
		float AmountOfHpHeal = 5.f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Special Parameters\|Switch Camp", meta = (ClampMin = 0.0))
		float RadiusToKillGrunts = 1500.f;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Special Parameters\|Switch Camp")
		bool IsSwitching = false;



};
