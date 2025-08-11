// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_AxeReturnHAttack.h"
#include "Kratos.h"

void UKS_AxeReturnHAttack::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	Me->CallAxe(GrabAxeTime, GrabAxeTime * 0.5f, true, -0.02f);
	Me->PlayMontage(EPlayerMontage::AxeReturnHAttack);
}

void UKS_AxeReturnHAttack::TickState(const FGenericStateParams& params, float DeltaTime)
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

void UKS_AxeReturnHAttack::ExitState(const FGenericStateParams& params)
{
	NextAttackType = EAttackType::None;
	Me->CanComboAttack = false;
}

void UKS_AxeReturnHAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_AxeReturnHAttack::HandleLAttack(const FGenericStateParams& params)
{
	NextAttackType = EAttackType::LAttack;
}

void UKS_AxeReturnHAttack::HandleHAttack(const FGenericStateParams& params)
{
	NextAttackType = EAttackType::HAttack;
}
