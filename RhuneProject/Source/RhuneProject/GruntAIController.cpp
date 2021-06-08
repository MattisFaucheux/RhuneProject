// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "BasicGruntCharacter.h"
#include "Fortress.h"
#include "RhuneProjectCharacter.h"

AGruntAIController::AGruntAIController()
{

    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("Perception"));

}

void AGruntAIController::BeginPlay()
{

	Super::BeginPlay();

	bool Success = RunBehaviorTree(DwarfBehavior);
	if (!Success)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(RefreshTargetTimer, this, &AGruntAIController::RefreshTarget, RefreshTargetFrequency, true);
}

void AGruntAIController::RefreshTarget()
{
	ABasicGruntCharacter* MyGrunt = nullptr;
	
	if (GetCharacter()->IsA<ABasicGruntCharacter>())
	{
		MyGrunt = Cast<ABasicGruntCharacter>(GetCharacter());
	}
	
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);

	if (ActorsWithTag.Num() == 0)
	{
		return;
	}

	AActor* AITarget = nullptr;
	float TargetDistance = 0.f;

	for (int index = 0; index < ActorsWithTag.Num(); index++)
	{
		AActor* AITargetTest = ActorsWithTag[index];
		float DistanceTo = GetPawn()->GetDistanceTo(AITargetTest);

		if(AITargetTest->IsA<AFortress>())
		{
			AFortress* Fortress = Cast<AFortress>(AITargetTest);

			if (MyGrunt)
			{
				if (Fortress->IsSameCamp(MyGrunt->GruntCamp))
				{
					continue;
				}
			}
		}
		
		if (AITarget == nullptr)
		{
			AITarget = ActorsWithTag[index];
			TargetDistance = DistanceTo;
		}
		else if (DistanceTo < TargetDistance)
		{
			AITarget = ActorsWithTag[index];
			TargetDistance = DistanceTo;
		}
	}

	GetBlackboardComponent()->SetValueAsObject(TargetActorKey, AITarget);
	GetWorldTimerManager().ClearTimer(RefreshTargetTimer);
}
