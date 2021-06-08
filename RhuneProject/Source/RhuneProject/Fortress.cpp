// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress.h"

#include "HpComponent.h"
#include "GruntSpawner.h"
#include "BasicGruntCharacter.h"
#include "GruntAIController.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AFortress::AFortress()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StaticMesh")));

	if (Mesh != nullptr)
	{
		SetRootComponent(Mesh);
	}

	//Create a Hp Component
	HpComponent = CreateDefaultSubobject<UHpComponent>(FName(TEXT("HpComponent")));
	
}

// Called when the game starts or when spawned
void AFortress::BeginPlay()
{
	Super::BeginPlay();

	SetSpawnerCamp(ActualFortressCamp);
}

// Called every frame
void AFortress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFortress::TakeDamages(float damages)
{
	if (HpComponent != nullptr)
	{
		HpComponent->TakeDamages(damages);
		GetWorldTimerManager().ClearTimer(HealLoopTimer);
		GetWorldTimerManager().SetTimer(HealRestartLoopTimer, this, &AFortress::StartHealLoop, 1000.f, false, TimeWithoutHitToRegenerate);

		if(HpComponent->IsDead())
		{
			SwitchCamp();
		}
	}
}

void AFortress::SwitchCamp()
{
	TArray<AActor*> FoundGrunts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicGruntCharacter::StaticClass(), FoundGrunts);

	for (int GruntIndex = 0; GruntIndex < FoundGrunts.Num(); GruntIndex++)
	{
		if (ABasicGruntCharacter* GruntCharacter = Cast<ABasicGruntCharacter>(FoundGrunts[GruntIndex]))
		{			
			if (GetDistanceTo(GruntCharacter) < RadiusToKillGrunts)
			{
				GruntCharacter->Kill();
			}
			else if(AGruntAIController* GruntAI = Cast<AGruntAIController>(GruntCharacter->GetController()))
			{
				GruntAI->RefreshTarget();
			}
		}
	}
	
	if(ActualFortressCamp == ActorCamp::AllyActor)
	{
		ActualFortressCamp = EnemyActor;
	}
	else
	{
		ActualFortressCamp = AllyActor;
	}

	SetSpawnerCamp(ActualFortressCamp);
	HpComponent->HealHpMax();

	IsSwitching = true;


}

float AFortress::GetNumberOfHp()
{
	return HpComponent->GetActualHp();
}

float AFortress::GetNumberOfHpMax()
{
	return HpComponent->GetMaxHp();
}


void AFortress::SetSpawnerCamp(TEnumAsByte<ActorCamp> FortressCamp)
{
	for (int SpawnerIndex = 0; SpawnerIndex < EnemiesSpawner.Num(); SpawnerIndex++)
	{
		AGruntSpawner* EnemySpawner = EnemiesSpawner[SpawnerIndex];
		if (EnemySpawner)
		{
			EnemySpawner->SetSpawnerCamp(FortressCamp);
		}
	}
}

void AFortress::HealFortress()
{
	HpComponent->HealHp(AmountOfHpHeal);

	if (HpComponent->ActualHp == HpComponent->MaxHp)
	{
		GetWorldTimerManager().ClearTimer(HealLoopTimer);
	}
}

void AFortress::StartHealLoop()
{
	GetWorldTimerManager().SetTimer(HealLoopTimer, this, &AFortress::HealFortress, FrequencyOfHeal, true, 0.f);
}


bool AFortress::IsSameCamp(TEnumAsByte<ActorCamp> OtherFortressCamp)
{
	if(ActualFortressCamp  == OtherFortressCamp)
	{
		return true;
	}
	else
	{
		return false;
	}
}
