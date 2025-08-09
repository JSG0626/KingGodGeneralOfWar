// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_AxeReturnLAttack.h"
#include "Kratos.h"

void UKS_AxeReturnLAttack::EnterState(const FGenericStateParams& params)
{
	Me->CallAxe(GrabAxeTime, GrabAxeTime, true, 0.05f);
	Me->PlayMontage(EPlayerMontage::AxeReturnLAttack);
}

void UKS_AxeReturnLAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	if (Me->CanComboAttack)
	{
		if (NextAttackType == EAttackType::LAttack)
		{
			Me->SetKratosState(EPlayerState::LAttack);
		}
		else if (NextAttackType == EAttackType::HAttack)
		{
			Me->SetKratosState(EPlayerState::HAttack);
		}
		Me->CanComboAttack = false;
	}
}

void UKS_AxeReturnLAttack::ExitState(const FGenericStateParams& params)
{
	Me->CanComboAttack = false;
	NextAttackType = EAttackType::None;
}

void UKS_AxeReturnLAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_AxeReturnLAttack::HandleLAttack(const FGenericStateParams& params)
{
	NextAttackType = EAttackType::LAttack;
}

void UKS_AxeReturnLAttack::HandleHAttack(const FGenericStateParams& params)
{
	NextAttackType = EAttackType::HAttack;
}
