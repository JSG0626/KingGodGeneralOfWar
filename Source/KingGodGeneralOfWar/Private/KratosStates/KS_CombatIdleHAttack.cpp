// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_CombatIdleHAttack.h"
#include "Kratos.h"

void UKS_CombatIdleHAttack::EnterState(const FGenericStateParams& params)
{
	Me->PlayMontage(EPlayerMontage::CombatIdleHAttack);
}

void UKS_CombatIdleHAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_CombatIdleHAttack::ExitState(const FGenericStateParams& params)
{
}

void UKS_CombatIdleHAttack::HandleDodge(const FGenericStateParams& params)
{
}

void UKS_CombatIdleHAttack::HandleLAttack(const FGenericStateParams& params)
{
}

void UKS_CombatIdleHAttack::HandleHAttack(const FGenericStateParams& params)
{
}

void UKS_CombatIdleHAttack::HandleAbility(const FGenericStateParams& params)
{
}
