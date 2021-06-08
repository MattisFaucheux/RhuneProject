// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"

#include "RhuneProjectCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ARhuneProjectCharacter* MyCharacter = Cast< ARhuneProjectCharacter>(TryGetPawnOwner());

	if (MyCharacter == nullptr)
	{
		return;
	}

	float MoveSpeed = MyCharacter->GetCharacterMovement()->Velocity.Size();
	
	IsWalking = (!FMath::IsNearlyZero(MoveSpeed));

	if (IsWalking)
	{
		float MaxSpeed = MyCharacter->GetCharacterMovement()->MaxWalkSpeed;
		WalkBlendRatio = (MoveSpeed / MaxSpeed) * 100.0f;
	}

	IsOcculteAura = (MyCharacter->AuraType == CharacterAuraType::Occulte);
}
