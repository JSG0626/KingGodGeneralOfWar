// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_HChargeAttack.h"
#include "Kratos.h"

void UKS_HChargeAttack::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	Me->PlayMontage(EPlayerMontage::HChargeAttack);
}

void UKS_HChargeAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_HChargeAttack::ExitState(const FGenericStateParams& params)
{
}

void UKS_HChargeAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_HChargeAttack::HandleHChargeAttackHit(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::HChargeAttackHit);
}
