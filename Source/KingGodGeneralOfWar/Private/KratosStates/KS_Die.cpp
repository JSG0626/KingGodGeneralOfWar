// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Die.h"
#include "Kratos.h"

void UKS_Die::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	StateLog(TEXT("Die Enter"));

	Me->PlayMontage(EPlayerMontage::Die);
	Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, 1);
}

void UKS_Die::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_Die::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Die Exit"));
}
