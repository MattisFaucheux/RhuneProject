// Copyright Epic Games, Inc. All Rights Reserved.

#include "RhuneProjectCharacter.h"


#include "BasicGruntCharacter.h"
#include "Fortress.h"
#include "ActorCamp.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "HeroAnimInstance.h"
#include "HpComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// ARhuneProjectCharacter

ARhuneProjectCharacter::ARhuneProjectCharacter()
{	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Create a Hp Component
	HpComponent = CreateDefaultSubobject<UHpComponent>(FName(TEXT("HpComponent")));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARhuneProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARhuneProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARhuneProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARhuneProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARhuneProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARhuneProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARhuneProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARhuneProjectCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARhuneProjectCharacter::StartBasicAttack);
	//PlayerInputComponent->BindAction("Attack", IE_Released, this, &ARhuneProjectCharacter::StopBasicAttack);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ARhuneProjectCharacter::StartDash);
	//PlayerInputComponent->BindAction("SwitchAura", IE_Pressed, this, &ARhuneProjectCharacter::SwitchAura);
}

void ARhuneProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsDoingAnAction && ActorsTouchedInLastAttack.Num() > 0)
	{
		ActorsTouchedInLastAttack.Empty();
	}
	
}

void ARhuneProjectCharacter::StartBasicAttack()
{
	if (!IsDoingAnAction)
	{
		if (AnimInstance != nullptr)
		{
			AnimInstance->IsAttacking = true;
			IsDoingAnAction = true;
		}
	}
}

void ARhuneProjectCharacter::StopBasicAttack()
{
	if (AnimInstance != nullptr)
	{
		AnimInstance->IsAttacking = false;
		IsDoingAnAction = false;
	}
}

void ARhuneProjectCharacter::StartDash()
{
	if (!IsDoingAnAction)
	{
		if (GetCharacterMovement()->IsMovingOnGround() && CanDoDash)
		{
			CanDoDash = false;
			IsDoingAnAction = true;

			FVector LaunchVelocity = GetCapsuleComponent()->GetForwardVector() * DashVelocity;
			LaunchCharacter(LaunchVelocity, false, false);

			AnimInstance->IsDashing = true;

			GetWorldTimerManager().SetTimer(DashDurationTimer, this, &ARhuneProjectCharacter::StopDash, 1000.0f, false, DashTime);
			
			InitialGroundFriction = GetCharacterMovement()->GroundFriction;
			InitialFallingLateralFriction = GetCharacterMovement()->FallingLateralFriction;
			
			GetCharacterMovement()->GroundFriction = 0;
			GetCharacterMovement()->FallingLateralFriction = 0;
		}
	}
}

void ARhuneProjectCharacter::StopDash()
{
	GetWorldTimerManager().ClearTimer(DashDurationTimer);
	GetCharacterMovement()->StopMovementImmediately();
	
	AnimInstance->IsDashing = false;
	IsDoingAnAction = false;

	GetCharacterMovement()->GroundFriction = InitialGroundFriction;
	GetCharacterMovement()->FallingLateralFriction = InitialFallingLateralFriction;

	GetWorldTimerManager().SetTimer(DashReloadTimer, this, &ARhuneProjectCharacter::SetCanDoDash, 1000.0f, false, DashReloadTime);
}

float ARhuneProjectCharacter::GetDashReloadTimeRemaining()
{
	return GetWorldTimerManager().GetTimerRemaining(DashReloadTimer);
}

float ARhuneProjectCharacter::GetAoeReloadTimeRemaining()
{
	return GetWorldTimerManager().GetTimerRemaining(AoeAttackReloadTimer);
}

float ARhuneProjectCharacter::GetLaserReloadTimeRemaining()
{
	return GetWorldTimerManager().GetTimerRemaining(LaserAttackReloadTimer);
}

float ARhuneProjectCharacter::GetSwitchAuraReloadTimeRemaining()
{
	return GetWorldTimerManager().GetTimerRemaining(SwitchAuraReloadTimer);
}

void ARhuneProjectCharacter::SetCanDoDash()
{
	CanDoDash = true;
}

void ARhuneProjectCharacter::SetCanDoAoeAttack()
{
	CanDoAoeAttack = true;
}

void ARhuneProjectCharacter::SetCanDoLaserAttack()
{
	CanDoLaserAttack = true;
}

void ARhuneProjectCharacter::SetCantDoLaserAttack()
{
	CanDoLaserAttack = false;
}

void ARhuneProjectCharacter::SetCantDoAoeAttack()
{
	CanDoAoeAttack = false;
}

void ARhuneProjectCharacter::SetCanSwitchAura()
{
	CanSwitchAura = true;
}

void ARhuneProjectCharacter::CallAoeAttackReload()
{
	GetWorldTimerManager().SetTimer(AoeAttackReloadTimer, this, &ARhuneProjectCharacter::SetCanDoAoeAttack, 1000.0f, false, AoeAttackReloadTime);
}

void ARhuneProjectCharacter::CallLaserAttackReload()
{
	GetWorldTimerManager().SetTimer(LaserAttackReloadTimer, this, &ARhuneProjectCharacter::SetCanDoLaserAttack, 1000.0f, false, LaserAttackReloadTime);
}

void ARhuneProjectCharacter::DoDamages(AActor* OtherActor, AttackType attackType)
{
	if(OtherActor == this)
	{
		return;
	}
	else if (OtherActor->IsA<AFortress>())
	{
		AFortress* Fortress = Cast<AFortress>(OtherActor);
		if (Fortress->IsSameCamp(ActorCamp::AllyActor))
		{
			return;
		}
	}

	for(int ActorsIndex = 0; ActorsIndex < ActorsTouchedInLastAttack.Num(); ActorsIndex++)
	{
		if(OtherActor == ActorsTouchedInLastAttack[ActorsIndex])
		{
			return;
		}
	}

	ActorsTouchedInLastAttack.Emplace(OtherActor);

	TSubclassOf<class UDamageType> Damageclass;
	float damages = 0.f;
	float damagesMultiplier = 1.f;

        if(OtherActor->IsA<ABasicGruntCharacter>()) 
		{
	         ABasicGruntCharacter* grunt = Cast<ABasicGruntCharacter>(OtherActor);
        	 if(grunt->IsSameCamp(ActorCamp::AllyActor))
        	 {
				damagesMultiplier = 0.f;
				return;
        	 }
	         if(grunt->IsSameAuraType(AuraType)) 
	         {
				damagesMultiplier = SameAuraDamagesMultiplier;
	         }
			 else
			 {
				damagesMultiplier = DifferentAuraDamagesMultiplier;
			 }
        }

	if(attackType == AttackType::BasicAttack)
	{
		damages = BasicAttackDamages;
		
	}
	else if(attackType == AttackType::LaserAttack)
	{
		CanDoLaserAttack = false;
		
		damages = LaserAttackDamages;
	}
	else if(attackType == AttackType::AoeAttack)
	{
		CanDoAoeAttack = false;
		
		damages = AoeAttackDamages;
	}
	UGameplayStatics::ApplyDamage(OtherActor, damages * damagesMultiplier, GetController(), this, Damageclass);
}

void ARhuneProjectCharacter::SwitchAura()
{
	if(!CanSwitchAura)
	{
		return;
	}

	if (!IsDoingAnAction)
	{
		IsDoingAnAction = true;

		if (AuraType == CharacterAuraType::Celeste)
		{
			AuraType = CharacterAuraType::Occulte;
		}
		else
		{
			AuraType = CharacterAuraType::Celeste;
		}

		CanSwitchAura = false;

		GetWorldTimerManager().SetTimer(SwitchAuraReloadTimer, this, &ARhuneProjectCharacter::SetCanSwitchAura, 1000.f, false, SwitchAuraReloadTime);

		IsDoingAnAction = false;
	}
}

void ARhuneProjectCharacter::HealCharacter()
{
	HpComponent->HealHp(AmountOfHpHeal);
	
	if(HpComponent->ActualHp == HpComponent->MaxHp)
	{
		GetWorldTimerManager().ClearTimer(HealLoopTimer);
	}
}

void ARhuneProjectCharacter::StartHealLoop()
{
	GetWorldTimerManager().SetTimer(HealLoopTimer, this, &ARhuneProjectCharacter::HealCharacter, FrequencyOfHeal, true, 0.f);
}

bool ARhuneProjectCharacter::IsSameAuraType(TEnumAsByte<CharacterAuraType> OtherAuraType)
{
	if(AuraType == OtherAuraType)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ARhuneProjectCharacter::TakeDamages(float damages)
{
	if (HpComponent == nullptr)
	{
		return;
	}

	HpComponent->TakeDamages(damages);
	GetWorldTimerManager().ClearTimer(HealLoopTimer);
	GetWorldTimerManager().SetTimer(HealRestartLoopTimer, this, &ARhuneProjectCharacter::StartHealLoop, 1000.f, false, TimeWithoutHitToRegenerate);

	if (HpComponent->IsDead())
	{
		if (AnimInstance != nullptr)
		{
			AnimInstance->IsDead = true;
		}
	}
}

float ARhuneProjectCharacter::GetNumberOfHp()
{
	return HpComponent->GetActualHp();
}

float ARhuneProjectCharacter::GetNumberOfHpMax()
{
	return HpComponent->GetMaxHp();
}

void ARhuneProjectCharacter::SetAnimInstance()
{
	AnimInstance = Cast<UHeroAnimInstance>(GetMesh()->GetAnimInstance());
}


void ARhuneProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARhuneProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARhuneProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARhuneProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARhuneProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARhuneProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARhuneProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
