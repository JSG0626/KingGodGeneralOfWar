// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_CombatIdleLAttack.h"
#include "Kratos.h"

void UKS_CombatIdleLAttack::EnterState(const FGenericStateParams& params)
{
	Me->PlayMontage(EPlayerMontage::CombatIdleLAttack);
}

void UKS_CombatIdleLAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_CombatIdleLAttack::ExitState(const FGenericStateParams& params)
{
}

void UKS_CombatIdleLAttack::HandleLAttack(const FGenericStateParams& params)
{
}

void UKS_CombatIdleLAttack::HandleHAttack(const FGenericStateParams& params)
{
}

void UKS_CombatIdleLAttack::HandleAbility(const FGenericStateParams& params)
{
}

void UKS_CombatIdleLAttack::HandleGuard(const FGenericStateParams& params)
{
	bGuardInputOn = true;
}
