// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_GrabAxe.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_GrabAxe::EnterState(const FGenericStateParams& params)
{
	Anim->PlayMontage(EPlayerMontage::GrabAxe);
}

void UKS_GrabAxe::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_GrabAxe::ExitState(const FGenericStateParams& params)
{
	Anim->bRecallAxe = false;
}
