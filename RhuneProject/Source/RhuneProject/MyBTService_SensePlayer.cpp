// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_SensePlayer.h"

#include "Perception/AIPerceptionComponent.h"

#include "AIController.h"
#include "BasicGruntCharacter.h"
#include "RhuneProjectCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

void UMyBTService_SensePlayer::TickNode(UBehaviorTreeComponent &OwnerComp,
    uint8 *NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* MyController = OwnerComp.GetAIOwner();
	UAIPerceptionComponent* AIPerception = nullptr;

	if (MyController == nullptr)
	{
		return;
	}
	ACharacter* MyCharacter = MyController->GetCharacter();
	if (MyCharacter == nullptr)
	{
		return;
	}

	ABasicGruntCharacter* MyGrunt = nullptr;

	if(MyController->GetCharacter()->IsA<ABasicGruntCharacter>())
	{
		MyGrunt = Cast<ABasicGruntCharacter>(MyController->GetCharacter());
	}
	
	AIPerception = MyController->FindComponentByClass<UAIPerceptionComponent>();

	if (AIPerception == nullptr)
	{
		return;
	}

	TArray<AActor*> PerceivedActors;
	AIPerception->GetCurrentlyPerceivedActors(PerceivedSense, PerceivedActors);

	if (PerceivedActors.Num() == 0)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return;
	}

	AActor* ClosestPerceivedActor = nullptr;
	float ClosestActorDistance = 0.f;

	for (int index = 0; index < PerceivedActors.Num(); index++)
	{
		AActor* PerceivedActorTest = PerceivedActors[index];
		float DistanceTo = MyCharacter->GetDistanceTo(PerceivedActorTest);

		if (PerceivedActorTest->IsA<ABasicGruntCharacter>())
		{
			ABasicGruntCharacter* PerceivedGrunt = Cast<ABasicGruntCharacter>(PerceivedActorTest);

			if (MyGrunt)
			{
				if (PerceivedGrunt->IsSameCamp(MyGrunt->GruntCamp))
				{
					continue;
				}
			}
		}
		else if(PerceivedActorTest->IsA<ARhuneProjectCharacter>())
		{
			ARhuneProjectCharacter* PerceivedCharacter = Cast<ARhuneProjectCharacter>(PerceivedActorTest);

			if (MyGrunt)
			{
				if (MyGrunt->IsSameCamp(ActorCamp::AllyActor))
				{
					continue;
				}
			}
		}

		if (ClosestPerceivedActor == nullptr)
		{
			ClosestPerceivedActor = PerceivedActors[index];
			ClosestActorDistance = DistanceTo;
		}
		else if (DistanceTo < ClosestActorDistance)
		{
			ClosestPerceivedActor = PerceivedActors[index];
			ClosestActorDistance = DistanceTo;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestPerceivedActor);
}
