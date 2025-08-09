// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_CombatIdleHAttack.h"
#include "Kratos.h"

void UKS_CombatIdleHAttack::EnterState(const FGenericStateParams& params)
{
	Me->PlayMontage(EPlayerMontage::CombatIdleHAttack);
	bGuardInputOn = false;
	AttackInputOn = EAttackType::None;
}

void UKS_CombatIdleHAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	if (Me->bAxeGone && bGuardInputOn)
	{
		if (AttackInputOn == EAttackType::LAttack)
		{
			Me->SetKratosState(EPlayerState::AxeReturnLAttack);
		}
		else if (AttackInputOn == EAttackType::HAttack)
		{
			Me->SetKratosState(EPlayerState::AxeReturnHAttack);
		}
	}
}

void UKS_CombatIdleHAttack::ExitState(const FGenericStateParams& params)
{
}

bool UKS_CombatIdleHAttack::CanHandleGuard() const
{
	return Me->bAxeGone;
}

void UKS_CombatIdleHAttack::HandleLAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::LAttack;
}

void UKS_CombatIdleHAttack::HandleHAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::HAttack;
}

void UKS_CombatIdleHAttack::HandleAbility(const FGenericStateParams& params)
{
}

void UKS_CombatIdleHAttack::HandleGuard(const FGenericStateParams& params)
{
	bGuardInputOn = true;
}
