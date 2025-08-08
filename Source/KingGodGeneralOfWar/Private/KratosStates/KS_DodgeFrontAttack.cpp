// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_DodgeFrontAttack.h"
#include "Kratos.h"

void UKS_DodgeFrontAttack::EnterState(const FGenericStateParams& params)
{
	Me->PlayMontage(EPlayerMontage::DodgeFrontAttack);
}

void UKS_DodgeFrontAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_DodgeFrontAttack::ExitState(const FGenericStateParams& params)
{
}

void UKS_DodgeFrontAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}
