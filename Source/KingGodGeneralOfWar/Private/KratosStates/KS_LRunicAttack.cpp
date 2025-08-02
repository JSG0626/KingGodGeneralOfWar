// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_LRunicAttack.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_LRunicAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("LRunicAttack Enter"));
	Me->CurrentAttackType = EAttackType::RUNE_ATTACK;

	FRotator rotate = Me->GetController()->GetControlRotation();
	rotate.Pitch = 0;
	Me->SetActorRotation(rotate);
	Anim->PlayMontage(EPlayerMontage::LRunicAttack);
}

void UKS_LRunicAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_LRunicAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("LRunicAttack Exit"));
	Anim->Montage_Stop(0);
}

void UKS_LRunicAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_LRunicAttack::HandleHit(const FGenericStateParams& params)
{
	const float CurHP = Me->SetHP(Me->CurHP - params.AttackParams.Damage);
	if (CurHP == 0)
	{
		HandleDie(params);
	}
}
