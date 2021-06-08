// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GruntAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RHUNEPROJECT_API UGruntAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dwarf)
		bool IsWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dwarf)
		bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dwarf)
		bool IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dwarf, meta = (ClampMin = 0.0, ClampMax = 100.0))
		float WalkBlendRatio = 0.f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
