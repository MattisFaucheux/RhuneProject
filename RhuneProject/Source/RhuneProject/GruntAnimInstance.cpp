// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAnimInstance.h"
#include "BasicGruntCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGruntAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABasicGruntCharacter* MyCharacter = Cast< ABasicGruntCharacter>(TryGetPawnOwner());

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
}
