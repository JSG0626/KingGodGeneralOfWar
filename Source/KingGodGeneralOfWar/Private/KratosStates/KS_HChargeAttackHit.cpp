// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_HChargeAttackHit.h"
#include "Kratos.h"

void UKS_HChargeAttackHit::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	Me->PlayMontage(EPlayerMontage::HChargeAttackHit);
}

void UKS_HChargeAttackHit::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_HChargeAttackHit::ExitState(const FGenericStateParams& params)
{
}
