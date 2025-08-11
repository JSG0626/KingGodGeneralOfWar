// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_DodgeFrontAttack.h"
#include "Kratos.h"

void UKS_DodgeFrontAttack::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	Me->PlayMontage(EPlayerMontage::DodgeFrontAttack);
	AttackInputOn = EAttackType::None;
}

void UKS_DodgeFrontAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	if (Me->CanComboAttack && AttackInputOn != EAttackType::None)
	{
		if (AttackInputOn == EAttackType::LAttack)
		{
			Me->SetKratosState(EPlayerState::LAttack);
		}
		else
		{
			Me->SetKratosState(EPlayerState::HAttack);
		}
		Me->CanComboAttack = false;
	}
}

void UKS_DodgeFrontAttack::ExitState(const FGenericStateParams& params)
{
}


void UKS_DodgeFrontAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_DodgeFrontAttack::HandleLAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::LAttack;
}

void UKS_DodgeFrontAttack::HandleHAttack(const FGenericStateParams& params)
{
	AttackInputOn = EAttackType::HAttack;
}
