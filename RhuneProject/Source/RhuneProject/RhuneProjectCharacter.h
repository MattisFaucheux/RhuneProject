// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CharacterAuraType.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RhuneProjectCharacter.generated.h"

class UHpComponent;
class UHeroAnimInstance;

UENUM()
enum AttackType
{
	BasicAttack,
	LaserAttack,
	AoeAttack
};

UCLASS(config=Game)
class ARhuneProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ARhuneProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaSeconds) override;

	//virtual void BeginPlay() override;

private:

	class UHeroAnimInstance* AnimInstance = nullptr;

	FTimerHandle DashDurationTimer;

	FTimerHandle DashReloadTimer;

	FTimerHandle AoeAttackReloadTimer;

	FTimerHandle LaserAttackReloadTimer;

	FTimerHandle SwitchAuraReloadTimer;

	FTimerHandle HealLoopTimer;

	FTimerHandle HealRestartLoopTimer;

	FTimerHandle StopBasicAttackTimer;

	float InitialGroundFriction;
	
	float InitialFallingLateralFriction;

	TArray<AActor*> ActorsTouchedInLastAttack;

protected:

	UFUNCTION(BlueprintCallable)
		float GetDashReloadTimeRemaining();

	UFUNCTION(BlueprintCallable)
		float GetAoeReloadTimeRemaining();

	UFUNCTION(BlueprintCallable)
		float GetLaserReloadTimeRemaining();

	UFUNCTION(BlueprintCallable)
		float GetSwitchAuraReloadTimeRemaining();
	
	UFUNCTION()
		void SetCanDoDash();

	UFUNCTION()
		void SetCanDoAoeAttack();

	UFUNCTION()
		void SetCanDoLaserAttack();

	UFUNCTION(BlueprintCallable)
		void SetCantDoLaserAttack();

	UFUNCTION(BlueprintCallable)
		void SetCantDoAoeAttack();

	UFUNCTION(BlueprintCallable)
		void SetCanSwitchAura();

	UFUNCTION(BlueprintCallable)
		void CallAoeAttackReload();

	UFUNCTION(BlueprintCallable)
		void CallLaserAttackReload();

	UFUNCTION(BlueprintCallable)
		void TakeDamages(float damages);

	UFUNCTION(BlueprintCallable)
		float GetNumberOfHp();

	UFUNCTION(BlueprintCallable)
		float GetNumberOfHpMax();

	UFUNCTION(BlueprintCallable)
		void SetAnimInstance();

	UFUNCTION(BlueprintCallable)
		void StartBasicAttack();

	UFUNCTION(BlueprintCallable)
		void StopBasicAttack();

	UFUNCTION(BlueprintCallable)
		void StartDash();

	UFUNCTION(BlueprintCallable)
		void StopDash();

	UFUNCTION(BlueprintCallable)
		void DoDamages(AActor* OtherActor, AttackType attackType);

	UFUNCTION(BlueprintCallable)
		void SwitchAura();

	UFUNCTION(BlueprintCallable)
		void HealCharacter();

	UFUNCTION(BlueprintCallable)
		void StartHealLoop();


public:
	UFUNCTION(BlueprintCallable)
		bool IsSameAuraType(TEnumAsByte<CharacterAuraType> OtherAuraType);
	
	/* Hp Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hp)
	        class UHpComponent* HpComponent;

	/* Aura type of character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills\|Aura")
		 TEnumAsByte<CharacterAuraType> AuraType;

	/* Number of damage the basic attack does without modifiers*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Attack", meta = (ClampMin = 0.0))
		float BasicAttackDamages;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills\|Attack", meta = (ClampMin = 0.0))
		float BasicAttackTime = 0.5f;

	/* Number of damage the laser beam attack does without modifiers*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Attack", meta = (ClampMin = 0.0))
		float LaserAttackDamages;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Attack", meta = (ClampMin = 0.0))
		float LaserAttackReloadTime = 3.f;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills\|Attack")
		bool CanDoLaserAttack = true;

	/* Number of damage the aoe attack does without modifiers*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Attack", meta = (ClampMin = 0.0))
		float AoeAttackDamages;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Attack", meta = (ClampMin = 0.0))
		float AoeAttackReloadTime = 3.f;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills\|Attack")
		bool CanDoAoeAttack = true;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Dash", meta = (ClampMin = 0.0))
		float DashVelocity = 10000.f;;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Dash", meta = (ClampMin = 0.0))
		float DashTime = 0.2f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Dash", meta = (ClampMin = 0.0))
		float DashReloadTime = 3.f;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills\|Dash")
		bool CanDoDash = true;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skills\|Dash")
		bool IsDoingAnAction = false;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Aura", meta = (ClampMin = 0.0))
		float DifferentAuraDamagesMultiplier = 1.5f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Aura", meta = (ClampMin = 0.0))
		float SameAuraDamagesMultiplier = 0.5f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Aura", meta = (ClampMin = 0.0))
		float SwitchAuraReloadTime = 1.f;

	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills\|Aura")
		bool CanSwitchAura = true;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Regeneration", meta = (ClampMin = 0.0))
		float TimeWithoutHitToRegenerate = 15.f;

	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Regeneration", meta = (ClampMin = 0.1))
		float FrequencyOfHeal = 2.5f;
	
	/**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills\|Regeneration", meta = (ClampMin = 0.0))
		float AmountOfHpHeal = 5.f;

};



