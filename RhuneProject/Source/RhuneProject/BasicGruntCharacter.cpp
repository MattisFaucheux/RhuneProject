// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGruntCharacter.h"

#include "Fortress.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "GruntAnimInstance.h"
#include "HpComponent.h"
#include "RhuneProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasicGruntCharacter::ABasicGruntCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a Hp Component
	HpComponent = CreateDefaultSubobject<UHpComponent>(FName(TEXT("HpComponent")));

}

void ABasicGruntCharacter::StartAttack()
{
	if(GruntAnim == nullptr)
	{
		GruntAnim = Cast<UGruntAnimInstance>(GetMesh()->GetAnimInstance());
	}

	if (GruntAnim != nullptr)
	{
		GruntAnim->IsAttacking = true;
		IsAttacking = true;
	}
}

void ABasicGruntCharacter::StopAttack()
{
	if (GruntAnim == nullptr)
	{
		GruntAnim = Cast<UGruntAnimInstance>(GetMesh()->GetAnimInstance());
	}
	
	if (GruntAnim != nullptr)
	{
		GruntAnim->IsAttacking = false;
		IsAttacking = false;
	}
}

// Called when the game starts or when spawned
void ABasicGruntCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABasicGruntCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicGruntCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicGruntCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);


	if (!HpComponent->IsDead())
	{
		 if(GetController<AAIController>()->GetBrainComponent())
		 {
			 GetController<AAIController>()->GetBrainComponent()->StartLogic();
		 }
	}
}

void ABasicGruntCharacter::TakeDamages(float damages, AActor* damageCauser)
{
	if(damages == 0.f)
	{
		return;
	}
	
	if (HpComponent != nullptr)
	{
		if(HpComponent->IsDead())
		{
			return;
		}
		
		HpComponent->TakeDamages(damages);

		if(damageCauser->IsA<ARhuneProjectCharacter>())
		{
			FVector dir = GetActorLocation() - damageCauser->GetActorLocation();
			dir.Z = 0.f;
			dir.Normalize();
			dir *= KnockbackTakedDistanceMultiplier;
			dir.Z = KnockbackTakedHeight;

			TakeKnockback(dir);
			
		}

		if (HpComponent->IsDead())
		{
			Kill();
		}
	}
}

float ABasicGruntCharacter::GetNumberOfHp()
{
	return HpComponent->GetActualHp();
}

float ABasicGruntCharacter::GetNumberOfHpMax()
{
	return HpComponent->GetMaxHp();
}

void ABasicGruntCharacter::SwitchAura()
{
	if (AuraType == CharacterAuraType::Celeste)
	{
		AuraType = CharacterAuraType::Occulte;
	}
	else
	{
		AuraType = CharacterAuraType::Celeste;
	}
}

void ABasicGruntCharacter::DoDamages(AActor* OtherActor)
{
	if(!IsAttacking ||HpComponent->IsDead() || OtherActor == this)
	{
		return;
	}

	if(OtherActor->IsA<ABasicGruntCharacter>())
	{
		ABasicGruntCharacter* OtherGrunt = Cast<ABasicGruntCharacter>(OtherActor);
		if(OtherGrunt->IsSameCamp(GruntCamp)) 
		{
		         return;
		}
	}
	else if (OtherActor->IsA<AFortress>())
	{
		AFortress* Fortress = Cast<AFortress>(OtherActor);
		if (Fortress->IsSameCamp(GruntCamp))
		{
			return;
		}
	}

	TSubclassOf<class UDamageType> Damageclass;
	float damagesMultiplier = 1.f;

	if (OtherActor->IsA<ARhuneProjectCharacter>())
	{
		if (GruntCamp != ActorCamp::AllyActor)
		{
			ARhuneProjectCharacter* player = Cast<ARhuneProjectCharacter>(OtherActor);
			if (player->IsSameAuraType(AuraType))
			{
				damagesMultiplier = BadAuraDamagesMultiplier;
			}
			else
			{
				damagesMultiplier = GoodAuraDamagesMultiplier;
			}
		}
		else
		{
			return;
		}
	}
	
	UGameplayStatics::ApplyDamage(OtherActor, BasicAttackDamages * damagesMultiplier, GetController(), this, Damageclass);
}

void ABasicGruntCharacter::TakeKnockback(FVector Direction)
{
	GetController<AAIController>()->GetBrainComponent()->StopLogic(FString(TEXT("Take knockback")));
	GetCharacterMovement()->StopMovementImmediately();
	LaunchCharacter(Direction, true, true);
}


void ABasicGruntCharacter::Kill()
{
	if(!HpComponent->IsDead())
	{
		HpComponent->TakeDamages(HpComponent->ActualHp);
	}
	
	if (GruntAnim == nullptr)
	{
		GruntAnim = Cast<UGruntAnimInstance>(GetMesh()->GetAnimInstance());
	}

	if (GruntAnim)
	{
		GruntAnim->IsDead = true;
	}

	GetController<AAIController>()->GetBrainComponent()->StopLogic(FString(TEXT("Is dead")));
	SetLifeSpan(3.0f);
}

bool ABasicGruntCharacter::IsSameAuraType(TEnumAsByte<CharacterAuraType> OtherAuraType)
{
	if (AuraType == OtherAuraType)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ABasicGruntCharacter::IsSameCamp(TEnumAsByte<::ActorCamp> OtherActorCamp)
{
        if(GruntCamp == OtherActorCamp) 
        {
		return true;
        }
	else 
	{
		return false;
	}
}


