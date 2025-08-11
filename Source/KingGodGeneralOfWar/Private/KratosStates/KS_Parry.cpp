// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Parry.h"
#include "Kratos.h"
#include "SG_Shield.h"
#include <Components/ArrowComponent.h>
#include <BaseEnemy.h>
#include <WeaponInterface.h>

void UKS_Parry::SetUp(AKratos* Kratos)
{
	UKratosState::SetUp(Kratos);
	Shield = Kratos->GetShield();
}

void UKS_Parry::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	StateLog(TEXT("Parry Enter"));
	Me->PlayMontage(EPlayerMontage::Parry);
	Shield->SetTargetScale(true);
	FEnemyAttackParams AttackParams = params.AttackParams;

	GetWorld()->SpawnActor<AActor>(ParryingLightFactory, Shield->LightPosition->GetComponentTransform())->AttachToActor(Shield, FAttachmentTransformRules::SnapToTargetIncludingScale);

	Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, 0.5f);
	if (AttackParams.bMelee)
	{
		if (AttackParams.Attacker)
		{
			ABaseEnemy* Enemy = Cast<ABaseEnemy>(AttackParams.Attacker);
			if (Enemy)
			{
				Enemy->GetDamage(FGenericAttackParams(Me, PARRY_DAMAGE, PARRY_STUN_DAMAGE, EAttackDirectionType::LEFT));
			}
			else
			{

			}
		}
	}
}

void UKS_Parry::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Parry Tick"), true);
}

void UKS_Parry::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Parry Exit"));
	Shield->SetTargetScale(false);
}

void UKS_Parry::HandleLAttack(const FGenericStateParams& params)
{
}

void UKS_Parry::HandleDodge(const FGenericStateParams& params)
{
}
