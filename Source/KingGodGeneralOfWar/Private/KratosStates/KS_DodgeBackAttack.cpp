// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_DodgeBackAttack.h"
#include "Kratos.h"

void UKS_DodgeBackAttack::EnterState(const FGenericStateParams& params)
{
	Me->PlayMontage(EPlayerMontage::DodgeBackAttack);
}

void UKS_DodgeBackAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_DodgeBackAttack::ExitState(const FGenericStateParams& params)
{
}

void UKS_DodgeBackAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}
