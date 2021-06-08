// Fill out your copyright notice in the Description page of Project Settings.


#include "HpComponent.h"

// Sets default values for this component's properties
UHpComponent::UHpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHpComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	ActualHp = MaxHp;	
}


// Called every frame
void UHpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHpComponent::GetMaxHp()
{
	return MaxHp;
}

float UHpComponent::GetActualHp()
{
	return ActualHp;
}

bool UHpComponent::IsDead()
{
	return ActualHp <= 0.0f;
}

void UHpComponent::TakeDamages(float NbOfDamages)
{	
	if(NbOfDamages > ActualHp)
	{
		ActualHp = 0.0f;
	}
	else
	{
		ActualHp -= NbOfDamages;
	}

	
	
}

void UHpComponent::HealHpMax()
{
	ActualHp = MaxHp;
}

void UHpComponent::HealHp(float NbOfHpHeal)
{
	ActualHp += NbOfHpHeal;
	if(ActualHp > MaxHp)
	{
		ActualHp = MaxHp;
	}
}

