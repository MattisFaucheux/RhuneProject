// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_SensePlayer.generated.h"

/**
 * 
 */
UCLASS()
class RHUNEPROJECT_API UMyBTService_SensePlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()



public:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = SenseService)
		TSubclassOf<class UAISense> PerceivedSense = nullptr;
};
