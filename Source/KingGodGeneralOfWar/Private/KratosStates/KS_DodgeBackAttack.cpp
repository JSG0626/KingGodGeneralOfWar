// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_DodgeBackAttack.h"
#include "Kratos.h"

void UKS_DodgeBackAttack::EnterState(const FGenericStateParams& params)
{
	Me->PlayMontage(EPlayerMontage::DodgeBackAttack);
	bGuardInputOn = false;
	AttackInputOn = EAttackType::None;
}

void UKS_DodgeBackAttack::TickState(const FGenericStateParams& params, float DeltaTime)
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

void UKS_DodgeBackAttack::ExitState(const FGenericStateParams& params)
{
}

bool UKS_DodgeBackAttack::CanHandleGuard() const
{
	return Me->bAxeGone;
}

void UKS_DodgeBackAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_DodgeBackAttack::HandleGuard(const FGenericStateParams& params)
{
	bGuardInputOn = true;
}

void UKS_DodgeBackAttack::HandleLAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::LAttack;
}

void UKS_DodgeBackAttack::HandleHAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::HAttack;
}
