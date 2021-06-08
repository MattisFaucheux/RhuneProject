// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopAttack.h"

#include "AIController.h"
#include "BasicGruntCharacter.h"

EBTNodeResult::Type UBTTask_StopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABasicGruntCharacter* Character = Cast<ABasicGruntCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character)
	{
		Character->StopAttack();
	}

	return EBTNodeResult::Succeeded;
}
