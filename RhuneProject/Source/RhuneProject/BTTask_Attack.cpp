// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "BasicGruntCharacter.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
    UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	ABasicGruntCharacter* Character = Cast<ABasicGruntCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character)
	{
		Character->StartAttack();
	}

	return EBTNodeResult::Succeeded;
}
