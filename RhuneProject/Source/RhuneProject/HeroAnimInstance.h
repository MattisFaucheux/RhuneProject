// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RHUNEPROJECT_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsDoingAoeAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsDoingBeamAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsDoingSwapAura = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsDashing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
		bool IsOcculteAura = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero, meta = (ClampMin = 0.0, ClampMax = 100.0))
		float WalkBlendRatio = 0.f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
