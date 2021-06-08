// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterAuraType.h"
#include "ActorCamp.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicGruntCharacter.generated.h"

class UHpComponent;
class UGruntAnimInstance;

UCLASS()
class RHUNEPROJECT_API ABasicGruntCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicGruntCharacter();

	void StartAttack();

	void StopAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

private:

	UGruntAnimInstance* GruntAnim = nullptr;
	
protected:

	UFUNCTION(BlueprintCallable)
		void TakeDamages(float damages, AActor* damageCauser);

	UFUNCTION(BlueprintCallable)
		float GetNumberOfHp();

	UFUNCTION(BlueprintCallable)
		float GetNumberOfHpMax();

	UFUNCTION(BlueprintCallable)
		void SwitchAura();

	UFUNCTION(BlueprintCallable)
		void DoDamages(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
		void TakeKnockback(FVector Direction);

public:
	UFUNCTION(BlueprintCallable)
		void Kill();

	UFUNCTION(BlueprintCallable)
		bool IsSameAuraType(TEnumAsByte<CharacterAuraType> OtherAuraType);

	UFUNCTION(BlueprintCallable)
		bool IsSameCamp(TEnumAsByte<ActorCamp> OtherActorCamp);

	/* Hp Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hp)
		class UHpComponent* HpComponent = nullptr;

	/* Aura type of character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
		TEnumAsByte<CharacterAuraType> AuraType;

	/* Actor camp of character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
		TEnumAsByte<ActorCamp> GruntCamp;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
		bool IsAttacking = false;;

	/* Number of damage the basic attack does without modifiers*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills", meta = (ClampMin = 0.0))
		float BasicAttackDamages = 5.f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills", meta = (ClampMin = 0.0))
		float GoodAuraDamagesMultiplier = 1.5f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills", meta = (ClampMin = 0.0))
		float BadAuraDamagesMultiplier = 0.5f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Knockback", meta = (ClampMin = 0.0))
		float KnockbackTakedDistanceMultiplier = 5.f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Knockback", meta = (ClampMin = 0.0))
		float KnockbackTakedHeight = 100.f;

};
