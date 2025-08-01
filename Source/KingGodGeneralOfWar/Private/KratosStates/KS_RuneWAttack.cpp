﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_RuneWAttack.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_RuneWAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("RuneWAttack Enter"));
	Me->CurrentAttackType = EAttackType::RUNE_ATTACK;

	FRotator rotate = Me->GetController()->GetControlRotation();
	rotate.Pitch = 0;
	Me->SetActorRotation(rotate);
	Anim->PlayMontage(EPlayerMontage::RuneWAttack);
}

void UKS_RuneWAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_RuneWAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("RuneWAttack Exit"));
	Anim->Montage_Stop(0);
}

void UKS_RuneWAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_RuneWAttack::HandleHit(const FGenericStateParams& params)
{
	const float CurHP = Me->SetHP(Me->CurHP - params.AttackParams.Damage);
	if (CurHP == 0)
	{
		HandleDie(params);
	}
}
