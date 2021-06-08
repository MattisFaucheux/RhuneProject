// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GruntAIController.generated.h"

/**
 * 
 */
class ABasicGruntCharacter;

UCLASS()
class RHUNEPROJECT_API AGruntAIController : public AAIController
{
	GENERATED_BODY()


public:

	AGruntAIController();

	virtual void BeginPlay() override;

	void RefreshTarget();


private:

	FTimerHandle RefreshTargetTimer;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GruntAI)
		class UBehaviorTree* DwarfBehavior = nullptr;

	/* Tag of the actor that we want to target*/
	UPROPERTY(EditDefaultsOnly, Category = GruntAI)
		FName TargetTag = "AITarget";


	UPROPERTY(VisibleDefaultsOnly, Category = GruntAI)
		FName TargetActorKey = "TargetActor";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GruntAI)
		float RefreshTargetFrequency = 2.5f;
	
};
