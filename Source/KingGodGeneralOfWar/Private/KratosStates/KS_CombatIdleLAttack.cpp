// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_CombatIdleLAttack.h"
#include "Kratos.h"

void UKS_CombatIdleLAttack::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	Me->PlayMontage(EPlayerMontage::CombatIdleLAttack);
	bGuardInputOn = false;
	AttackInputOn = EAttackType::None;
}

void UKS_CombatIdleLAttack::TickState(const FGenericStateParams& params, float DeltaTime)
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

void UKS_CombatIdleLAttack::ExitState(const FGenericStateParams& params)
{
}

bool UKS_CombatIdleLAttack::CanHandleGuard() const
{
	return Me->bAxeGone;
}

void UKS_CombatIdleLAttack::HandleLAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::LAttack;
}

void UKS_CombatIdleLAttack::HandleHAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::HAttack;
}

void UKS_CombatIdleLAttack::HandleAbility(const FGenericStateParams& params)
{
}

void UKS_CombatIdleLAttack::HandleGuard(const FGenericStateParams& params)
{
	bGuardInputOn = true;
}
