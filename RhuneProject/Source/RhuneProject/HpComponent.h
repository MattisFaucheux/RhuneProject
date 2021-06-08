// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HpComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RHUNEPROJECT_API UHpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	UPROPERTY(EditAnywhere, Category = HpComponent, meta = (ClampMin = 0.0))
		float MaxHp = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HPComponent)
		float ActualHp = 0.f;

	UFUNCTION()
		float GetMaxHp();

	UFUNCTION()
		float GetActualHp();

	UFUNCTION(BlueprintCallable)
		bool IsDead();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
		void TakeDamages(float NbOfDamages);

	UFUNCTION(BlueprintCallable, BlueprintSetter)
		void HealHpMax();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
		void HealHp(float NbOfHpHeal);

	

		
};
